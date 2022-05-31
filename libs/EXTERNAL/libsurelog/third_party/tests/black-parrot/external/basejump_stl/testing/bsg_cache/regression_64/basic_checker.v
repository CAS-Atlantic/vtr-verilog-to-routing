module basic_checker
  import bsg_cache_pkg::*;
  #(parameter data_width_p="inv"
    , parameter addr_width_p="inv"
    , parameter mem_size_p="inv"

    , parameter data_mask_width_lp=(data_width_p>>3)
    , parameter cache_pkt_width_lp= `bsg_cache_pkt_width(addr_width_p,data_width_p)
  ) 
  (
    input clk_i
    , input reset_i
      
    , input en_i

    , input [cache_pkt_width_lp-1:0] cache_pkt_i
    , input v_i
    , input ready_o

    , input [data_width_p-1:0] data_o
    , input v_o
    , input yumi_i
  );


  `declare_bsg_cache_pkt_s(addr_width_p,data_width_p);

  localparam lg_data_size_in_bytes_lp = `BSG_SAFE_CLOG2(data_width_p/8);
  localparam lg_data_size_in_half_lp = `BSG_SAFE_CLOG2(data_width_p/16);
  localparam lg_data_size_in_words_lp = `BSG_SAFE_CLOG2(data_width_p/32);
  localparam lg_data_size_in_dwords_lp = `BSG_SAFE_CLOG2(data_width_p/64);

  bsg_cache_pkt_s cache_pkt;
  assign cache_pkt = cache_pkt_i;

  logic [data_width_p-1:0] shadow_mem [mem_size_p-1:0];
  logic [data_width_p-1:0] result [*];

  wire [addr_width_p-1:0] cache_pkt_word_addr = cache_pkt.addr[addr_width_p-1:lg_data_size_in_bytes_lp];

  // store logic
  logic [data_width_p-1:0] load_data, load_data_final;
  logic [data_width_p-1:0] store_pre_data;
  logic [data_width_p-1:0] store_data;
  logic [data_mask_width_lp-1:0] store_mask;

  always_comb begin
    case (cache_pkt.opcode)
      // Arithmetic ops need to be applied to individual segments
      AMOADD_W: begin
        store_pre_data = cache_pkt.data[0+:32] + load_data_final[0+:32];
      end
      AMOADD_D: begin
        store_pre_data = cache_pkt.data[0+:64] + load_data_final[0+:64];
      end
      AMOMIN_W: begin
        store_pre_data = (cache_pkt.data[0+:32] < load_data_final[0+:32])
          ? cache_pkt.data[0+:32]
          : load_data_final[0+:32];
      end
      AMOMIN_D: begin
        store_pre_data = (cache_pkt.data[0+:64] < load_data_final[0+:64])
          ? cache_pkt.data[0+:64]
          : load_data_final[0+:64];
      end
      AMOMAX_W: begin
        store_pre_data = (cache_pkt.data[0+:32] > load_data_final[0+:32])
          ? cache_pkt.data[0+:32]
          : load_data_final[0+:32];
      end
      AMOMAX_D: begin
        store_pre_data = (cache_pkt.data[0+:64] > load_data_final[0+:64])
          ? cache_pkt.data[0+:64]
          : load_data_final[0+:64];
      end
      AMOMINU_W: begin
        store_pre_data = ($unsigned(cache_pkt.data[0+:32]) < $unsigned(load_data_final[0+:32]))
          ? cache_pkt.data[0+:32]
          : load_data_final[0+:32];
      end
      AMOMINU_D: begin
        store_pre_data = ($unsigned(cache_pkt.data[0+:64]) < $unsigned(load_data_final[0+:64]))
          ? cache_pkt.data[0+:64]
          : load_data_final[0+:64];
      end
      AMOMAXU_W: begin
        store_pre_data = ($unsigned(cache_pkt.data[0+:32]) > $unsigned(load_data_final[0+:32]))
          ? cache_pkt.data[0+:32]
          : load_data_final[0+:32];
      end
      AMOMAXU_D: begin
        store_pre_data = ($unsigned(cache_pkt.data[0+:64]) > $unsigned(load_data_final[0+:64]))
          ? cache_pkt.data[0+:64]
          : load_data_final[0+:64];
      end

      AMOXOR_W, AMOXOR_D: begin
        store_pre_data = cache_pkt.data ^ load_data_final;
      end
      AMOAND_W, AMOAND_D: begin
        store_pre_data = cache_pkt.data & load_data_final;
      end
      AMOOR_W, AMOOR_D: begin
        store_pre_data = cache_pkt.data | load_data_final;
      end
      default: begin
        store_pre_data = cache_pkt.data;
      end
    endcase
  end

  always_comb begin
    case (cache_pkt.opcode)
      
      SD, AMOSWAP_D, AMOADD_D, AMOXOR_D
      ,AMOAND_D, AMOOR_D, AMOMIN_D
      ,AMOMAX_D, AMOMINU_D, AMOMAXU_D: begin
        store_data = {(data_width_p/64){store_pre_data[63:0]}};
        if (data_width_p > 64)
          store_mask = 8'b1111_1111 << (cache_pkt.addr[3+:lg_data_size_in_dwords_lp]*8);
        else
          store_mask = 8'b1111_1111;
      end

      SW, AMOSWAP_W, AMOADD_W, AMOXOR_W
      ,AMOAND_W, AMOOR_W, AMOMIN_W
      ,AMOMAX_W, AMOMINU_W, AMOMAXU_W: begin
        store_data = {(data_width_p/32){store_pre_data[31:0]}};
        store_mask = 4'b1111 << (cache_pkt.addr[2+:lg_data_size_in_words_lp]*4);
      end

      SH: begin
        store_data = {(data_width_p/16){store_pre_data[15:0]}};
        store_mask = 2'b11 << (cache_pkt.addr[1+:lg_data_size_in_half_lp]*2);
      end

      SB: begin
        store_data = {(data_width_p/8){store_pre_data[7:0]}};
        store_mask = 1'b1 << cache_pkt.addr[0+:lg_data_size_in_bytes_lp];
      end

      SM: begin
        store_data = store_pre_data;
        store_mask = cache_pkt.mask;
      end

      default: begin
        store_data = '0;
        store_mask = '0;
      end
    endcase
  end

  // load logic
  logic [7:0] byte_sel;
  logic [15:0] half_sel;
  logic [31:0] word_sel;
  logic [63:0] dword_sel;

  assign load_data = shadow_mem[cache_pkt_word_addr];

  bsg_mux #(
    .els_p(data_width_p/8)
    ,.width_p(8)
  ) byte_mux (
    .data_i(load_data)
    ,.sel_i(cache_pkt.addr[0+:lg_data_size_in_bytes_lp])
    ,.data_o(byte_sel)
  );

  bsg_mux #(
    .els_p(data_width_p/16)
    ,.width_p(16)
  ) half_mux (
    .data_i(load_data)
    ,.sel_i(cache_pkt.addr[1+:lg_data_size_in_half_lp])
    ,.data_o(half_sel)
  );

  bsg_mux #(
    .els_p(data_width_p/32)
    ,.width_p(32)
  ) word_mux (
    .data_i(load_data)
    ,.sel_i(cache_pkt.addr[2+:lg_data_size_in_words_lp])
    ,.data_o(word_sel)
  );

  bsg_mux #(
    .els_p(data_width_p/64)
    ,.width_p(64)
  ) dword_mux (
    .data_i(load_data)
    ,.sel_i(cache_pkt.addr[3+:lg_data_size_in_dwords_lp])
    ,.data_o(dword_sel)
  );

  always_comb begin
    case (cache_pkt.opcode)
      LD, AMOSWAP_D, AMOADD_D, AMOXOR_D
      ,AMOAND_D, AMOOR_D, AMOMIN_D
      ,AMOMAX_D, AMOMINU_D, AMOMAXU_D: load_data_final = {{(data_width_p-64){dword_sel[63]}}, dword_sel};
      LW, AMOSWAP_W, AMOADD_W, AMOXOR_W
      ,AMOAND_W, AMOOR_W, AMOMIN_W
      ,AMOMAX_W, AMOMINU_W, AMOMAXU_W: load_data_final = {{(data_width_p-32){word_sel[31]}}, word_sel};
      LH: load_data_final = {{(data_width_p-16){half_sel[15]}}, half_sel};
      LB: load_data_final = {{(data_width_p-8){byte_sel[7]}}, byte_sel};
      LWU: load_data_final = {{(data_width_p-32){1'b0}}, word_sel};
      LHU: load_data_final = {{(data_width_p-16){1'b0}}, half_sel};
      LBU: load_data_final = {{(data_width_p-8){1'b0}}, byte_sel};
      LM: begin
        for (integer i = 0; i < data_mask_width_lp; i++)
          if (cache_pkt.mask[i])
            load_data_final[8*i+:8] = load_data[8*i+:8];
          else
            load_data_final[8*i+:8] = 8'h0;
      end
      default: load_data_final = '0;
    endcase
  end



  integer send_id, recv_id;

  always_ff @ (posedge clk_i) begin
    if (reset_i) begin
      send_id <= '0;
      recv_id <= '0;
      for (integer i = 0; i < mem_size_p; i++)
        shadow_mem[i] = '0;
    end
    else begin

      if (en_i) begin

        // input recorder
        if (v_i & ready_o) begin
          case (cache_pkt.opcode)

            TAGST: begin
              result[send_id] = '0;
              send_id++;
            end

            LM, LD, LW, LH, LB, LWU, LHU, LBU: begin
              result[send_id] = load_data_final;
              send_id++;
            end

            SM, SD, SW, SH, SB: begin
              result[send_id] = '0;
              send_id++;
              for (integer i = 0; i < data_mask_width_lp; i++)
                if (store_mask[i])
                  shadow_mem[cache_pkt_word_addr][8*i+:8] <= store_data[8*i+:8];
            end

            AMOSWAP_W, AMOADD_W, AMOXOR_W,
            AMOAND_W, AMOOR_W, AMOMIN_W,
            AMOMAX_W, AMOMINU_W, AMOMAXU_W,
            AMOSWAP_D, AMOADD_D, AMOXOR_D,
            AMOAND_D, AMOOR_D, AMOMIN_D,
            AMOMAX_D, AMOMINU_D, AMOMAXU_D: begin
              result[send_id] = load_data_final;
              send_id++;
              for (integer i = 0; i < data_mask_width_lp; i++)
                if (store_mask[i])
                  shadow_mem[cache_pkt_word_addr][8*i+:8] <= store_data[8*i+:8];
            end

            ALOCK, AUNLOCK, TAGFL, AFLINV: begin
              result[send_id] = '0;
              send_id++;
            end
          endcase
        end

        // output checker
        if (v_o & yumi_i) begin
          assert(result[recv_id] == data_o)
            else $fatal("[BSG_FATAL] output does not match expected result. Id=%d, Expected: %x. Actual: %x.",
                    recv_id, result[recv_id], data_o);
          recv_id++;
        end
      end
    end
  end

  
endmodule
