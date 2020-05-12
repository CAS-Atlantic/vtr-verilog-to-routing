`define WIDTH 8

module simple_op(a,b,out);
    input   [`WIDTH-1:0]a;
    input   [`WIDTH-1:0]b;
    output  [`WIDTH-1:0]out;
    wire    [`WIDTH-1:0]o;

genvar i;
generate
    for (i = 0; i < `WIDTH;i = i + 1) begin
      nor_node NNOR(                            //Change ***_node N***
        .in(a[i]),.inn(b[i]),
        .out(o[i]));
    end
  endgenerate
  assign out = o;
endmodule

module nor_node(in,inn,out); //Change the title
  input in;
  input inn;
  output out;
  wire temp;
  
  nor(temp,in,inn);         //Change function 
  assign out = temp;
  
endmodule 