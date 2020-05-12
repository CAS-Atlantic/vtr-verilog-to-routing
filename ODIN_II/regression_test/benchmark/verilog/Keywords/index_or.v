`define WIDTH 8

module simple_op(a,b,out);
    input   [`WIDTH-1:0]a;
    input   [`WIDTH-1:0]b;
    output  [`WIDTH-1:0]out;
    wire    [`WIDTH-1:0]o;

genvar i;
generate
    for (i = 0; i < `WIDTH;i = i + 1) begin
      or_node NOR(                            //Change ***_node N***
        .in(a[i]),.inn(b[i]),
        .out(o[i]));
    end
  endgenerate
  assign out = o;
endmodule

module or_node(in,inn,out); //Change the title
  input in;
  input inn;
  output out;
  wire temp;
  
  or(temp,in,inn);
  assign out = temp;
  
endmodule 