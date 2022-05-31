// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Register Top module auto-generated by `reggen`

`include "prim_assert.sv"

module clkmgr_reg_top (
  input clk_i,
  input rst_ni,

  // Below Regster interface can be changed
  input  tlul_pkg::tl_h2d_t tl_i,
  output tlul_pkg::tl_d2h_t tl_o,
  // To HW
  output clkmgr_reg_pkg::clkmgr_reg2hw_t reg2hw, // Write
  input  clkmgr_reg_pkg::clkmgr_hw2reg_t hw2reg, // Read

  // Config
  input devmode_i // If 1, explicit error return for unmapped register access
);

  import clkmgr_reg_pkg::* ;

  localparam int AW = 4;
  localparam int DW = 32;
  localparam int DBW = DW/8;                    // Byte Width

  // register signals
  logic           reg_we;
  logic           reg_re;
  logic [AW-1:0]  reg_addr;
  logic [DW-1:0]  reg_wdata;
  logic [DBW-1:0] reg_be;
  logic [DW-1:0]  reg_rdata;
  logic           reg_error;

  logic          addrmiss, wr_err;

  logic [DW-1:0] reg_rdata_next;

  tlul_pkg::tl_h2d_t tl_reg_h2d;
  tlul_pkg::tl_d2h_t tl_reg_d2h;

  assign tl_reg_h2d = tl_i;
  assign tl_o       = tl_reg_d2h;

  tlul_adapter_reg #(
    .RegAw(AW),
    .RegDw(DW)
  ) u_reg_if (
    .clk_i,
    .rst_ni,

    .tl_i (tl_reg_h2d),
    .tl_o (tl_reg_d2h),

    .we_o    (reg_we),
    .re_o    (reg_re),
    .addr_o  (reg_addr),
    .wdata_o (reg_wdata),
    .be_o    (reg_be),
    .rdata_i (reg_rdata),
    .error_i (reg_error)
  );

  assign reg_rdata = reg_rdata_next ;
  assign reg_error = (devmode_i & addrmiss) | wr_err ;

  // Define SW related signals
  // Format: <reg>_<field>_{wd|we|qs}
  //        or <reg>_{wd|we|qs} if field == 1 or 0
  logic clk_enables_clk_io_div4_peri_en_qs;
  logic clk_enables_clk_io_div4_peri_en_wd;
  logic clk_enables_clk_io_div4_peri_en_we;
  logic clk_enables_clk_usb_peri_en_qs;
  logic clk_enables_clk_usb_peri_en_wd;
  logic clk_enables_clk_usb_peri_en_we;
  logic clk_hints_clk_main_aes_hint_qs;
  logic clk_hints_clk_main_aes_hint_wd;
  logic clk_hints_clk_main_aes_hint_we;
  logic clk_hints_clk_main_hmac_hint_qs;
  logic clk_hints_clk_main_hmac_hint_wd;
  logic clk_hints_clk_main_hmac_hint_we;
  logic clk_hints_clk_main_kmac_hint_qs;
  logic clk_hints_clk_main_kmac_hint_wd;
  logic clk_hints_clk_main_kmac_hint_we;
  logic clk_hints_clk_main_otbn_hint_qs;
  logic clk_hints_clk_main_otbn_hint_wd;
  logic clk_hints_clk_main_otbn_hint_we;
  logic clk_hints_status_clk_main_aes_val_qs;
  logic clk_hints_status_clk_main_hmac_val_qs;
  logic clk_hints_status_clk_main_kmac_val_qs;
  logic clk_hints_status_clk_main_otbn_val_qs;

  // Register instances
  // R[clk_enables]: V(False)

  //   F[clk_io_div4_peri_en]: 0:0
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_enables_clk_io_div4_peri_en (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_enables_clk_io_div4_peri_en_we),
    .wd     (clk_enables_clk_io_div4_peri_en_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_enables.clk_io_div4_peri_en.q ),

    // to register interface (read)
    .qs     (clk_enables_clk_io_div4_peri_en_qs)
  );


  //   F[clk_usb_peri_en]: 1:1
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_enables_clk_usb_peri_en (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_enables_clk_usb_peri_en_we),
    .wd     (clk_enables_clk_usb_peri_en_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_enables.clk_usb_peri_en.q ),

    // to register interface (read)
    .qs     (clk_enables_clk_usb_peri_en_qs)
  );


  // R[clk_hints]: V(False)

  //   F[clk_main_aes_hint]: 0:0
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_hints_clk_main_aes_hint (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_hints_clk_main_aes_hint_we),
    .wd     (clk_hints_clk_main_aes_hint_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_hints.clk_main_aes_hint.q ),

    // to register interface (read)
    .qs     (clk_hints_clk_main_aes_hint_qs)
  );


  //   F[clk_main_hmac_hint]: 1:1
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_hints_clk_main_hmac_hint (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_hints_clk_main_hmac_hint_we),
    .wd     (clk_hints_clk_main_hmac_hint_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_hints.clk_main_hmac_hint.q ),

    // to register interface (read)
    .qs     (clk_hints_clk_main_hmac_hint_qs)
  );


  //   F[clk_main_kmac_hint]: 2:2
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_hints_clk_main_kmac_hint (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_hints_clk_main_kmac_hint_we),
    .wd     (clk_hints_clk_main_kmac_hint_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_hints.clk_main_kmac_hint.q ),

    // to register interface (read)
    .qs     (clk_hints_clk_main_kmac_hint_qs)
  );


  //   F[clk_main_otbn_hint]: 3:3
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h1)
  ) u_clk_hints_clk_main_otbn_hint (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (clk_hints_clk_main_otbn_hint_we),
    .wd     (clk_hints_clk_main_otbn_hint_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.clk_hints.clk_main_otbn_hint.q ),

    // to register interface (read)
    .qs     (clk_hints_clk_main_otbn_hint_qs)
  );


  // R[clk_hints_status]: V(False)

  //   F[clk_main_aes_val]: 0:0
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RO"),
    .RESVAL  (1'h1)
  ) u_clk_hints_status_clk_main_aes_val (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    .we     (1'b0),
    .wd     ('0  ),

    // from internal hardware
    .de     (hw2reg.clk_hints_status.clk_main_aes_val.de),
    .d      (hw2reg.clk_hints_status.clk_main_aes_val.d ),

    // to internal hardware
    .qe     (),
    .q      (),

    // to register interface (read)
    .qs     (clk_hints_status_clk_main_aes_val_qs)
  );


  //   F[clk_main_hmac_val]: 1:1
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RO"),
    .RESVAL  (1'h1)
  ) u_clk_hints_status_clk_main_hmac_val (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    .we     (1'b0),
    .wd     ('0  ),

    // from internal hardware
    .de     (hw2reg.clk_hints_status.clk_main_hmac_val.de),
    .d      (hw2reg.clk_hints_status.clk_main_hmac_val.d ),

    // to internal hardware
    .qe     (),
    .q      (),

    // to register interface (read)
    .qs     (clk_hints_status_clk_main_hmac_val_qs)
  );


  //   F[clk_main_kmac_val]: 2:2
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RO"),
    .RESVAL  (1'h1)
  ) u_clk_hints_status_clk_main_kmac_val (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    .we     (1'b0),
    .wd     ('0  ),

    // from internal hardware
    .de     (hw2reg.clk_hints_status.clk_main_kmac_val.de),
    .d      (hw2reg.clk_hints_status.clk_main_kmac_val.d ),

    // to internal hardware
    .qe     (),
    .q      (),

    // to register interface (read)
    .qs     (clk_hints_status_clk_main_kmac_val_qs)
  );


  //   F[clk_main_otbn_val]: 3:3
  prim_subreg #(
    .DW      (1),
    .SWACCESS("RO"),
    .RESVAL  (1'h1)
  ) u_clk_hints_status_clk_main_otbn_val (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    .we     (1'b0),
    .wd     ('0  ),

    // from internal hardware
    .de     (hw2reg.clk_hints_status.clk_main_otbn_val.de),
    .d      (hw2reg.clk_hints_status.clk_main_otbn_val.d ),

    // to internal hardware
    .qe     (),
    .q      (),

    // to register interface (read)
    .qs     (clk_hints_status_clk_main_otbn_val_qs)
  );




  logic [2:0] addr_hit;
  always_comb begin
    addr_hit = '0;
    addr_hit[0] = (reg_addr == CLKMGR_CLK_ENABLES_OFFSET);
    addr_hit[1] = (reg_addr == CLKMGR_CLK_HINTS_OFFSET);
    addr_hit[2] = (reg_addr == CLKMGR_CLK_HINTS_STATUS_OFFSET);
  end

  assign addrmiss = (reg_re || reg_we) ? ~|addr_hit : 1'b0 ;

  // Check sub-word write is permitted
  always_comb begin
    wr_err = 1'b0;
    if (addr_hit[0] && reg_we && (CLKMGR_PERMIT[0] != (CLKMGR_PERMIT[0] & reg_be))) wr_err = 1'b1 ;
    if (addr_hit[1] && reg_we && (CLKMGR_PERMIT[1] != (CLKMGR_PERMIT[1] & reg_be))) wr_err = 1'b1 ;
    if (addr_hit[2] && reg_we && (CLKMGR_PERMIT[2] != (CLKMGR_PERMIT[2] & reg_be))) wr_err = 1'b1 ;
  end

  assign clk_enables_clk_io_div4_peri_en_we = addr_hit[0] & reg_we & ~wr_err;
  assign clk_enables_clk_io_div4_peri_en_wd = reg_wdata[0];

  assign clk_enables_clk_usb_peri_en_we = addr_hit[0] & reg_we & ~wr_err;
  assign clk_enables_clk_usb_peri_en_wd = reg_wdata[1];

  assign clk_hints_clk_main_aes_hint_we = addr_hit[1] & reg_we & ~wr_err;
  assign clk_hints_clk_main_aes_hint_wd = reg_wdata[0];

  assign clk_hints_clk_main_hmac_hint_we = addr_hit[1] & reg_we & ~wr_err;
  assign clk_hints_clk_main_hmac_hint_wd = reg_wdata[1];

  assign clk_hints_clk_main_kmac_hint_we = addr_hit[1] & reg_we & ~wr_err;
  assign clk_hints_clk_main_kmac_hint_wd = reg_wdata[2];

  assign clk_hints_clk_main_otbn_hint_we = addr_hit[1] & reg_we & ~wr_err;
  assign clk_hints_clk_main_otbn_hint_wd = reg_wdata[3];





  // Read data return
  always_comb begin
    reg_rdata_next = '0;
    unique case (1'b1)
      addr_hit[0]: begin
        reg_rdata_next[0] = clk_enables_clk_io_div4_peri_en_qs;
        reg_rdata_next[1] = clk_enables_clk_usb_peri_en_qs;
      end

      addr_hit[1]: begin
        reg_rdata_next[0] = clk_hints_clk_main_aes_hint_qs;
        reg_rdata_next[1] = clk_hints_clk_main_hmac_hint_qs;
        reg_rdata_next[2] = clk_hints_clk_main_kmac_hint_qs;
        reg_rdata_next[3] = clk_hints_clk_main_otbn_hint_qs;
      end

      addr_hit[2]: begin
        reg_rdata_next[0] = clk_hints_status_clk_main_aes_val_qs;
        reg_rdata_next[1] = clk_hints_status_clk_main_hmac_val_qs;
        reg_rdata_next[2] = clk_hints_status_clk_main_kmac_val_qs;
        reg_rdata_next[3] = clk_hints_status_clk_main_otbn_val_qs;
      end

      default: begin
        reg_rdata_next = '1;
      end
    endcase
  end

  // Assertions for Register Interface
  `ASSERT_PULSE(wePulse, reg_we)
  `ASSERT_PULSE(rePulse, reg_re)

  `ASSERT(reAfterRv, $rose(reg_re || reg_we) |=> tl_o.d_valid)

  `ASSERT(en2addrHit, (reg_we || reg_re) |-> $onehot0(addr_hit))

  // this is formulated as an assumption such that the FPV testbenches do disprove this
  // property by mistake
  `ASSUME(reqParity, tl_reg_h2d.a_valid |-> tl_reg_h2d.a_user.parity_en == 1'b0)

endmodule
