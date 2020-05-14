
module simple_op(out);
    output  out;
    wire    o;

genvar i;
generate
    for (i = 0; i < 1;i = i + 1) begin
      assign o[i] = 0;
    end
  endgenerate
  assign out = o;
endmodule
