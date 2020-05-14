
module simple_op(out);
    output  [`WIDTH-1:0]out;
    wire    [`WIDTH-1:0]o;

genvar i;
generate
    for (i = 0; i < `WIDTH;i = i + 1) begin
      assign o[i] = 0;
    end
  endgenerate
  assign out = o;
endmodule
