`define WIDTH 8

module simple_op(a,out);
    input   [`WIDTH-1:0]a;
    output  [`WIDTH-1:0]out;
    wire    [`WIDTH-1:0]o;

genvar i;
generate
    for (i = 0; i < `WIDTH;i = i + 1) begin
      not_node NNOT(
        .in(a[i]),
        .out(o[i]));
    end
  endgenerate
  assign out = o;
endmodule

module not_node(in,out);
  input in;
  output out;
  wire temp;
  
  not(temp,in);
  assign out = temp;
  
endmodule 