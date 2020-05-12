module simple_op(out1,out2,out3,in);
  input  [1:0] in;
  output [1:0] out1;
  output [1:0] out2;
  output [1:0] out3;

  reg[1:0] a,b;

  initial begin 
    a = 2'b10;
    b = 2'b00;
  end 

  assign out1 = a;
  assign out2 = b;
  assign out3 = in;

endmodule 

//Error: Could not hook up the pin simple_op^BUF~0: not available.