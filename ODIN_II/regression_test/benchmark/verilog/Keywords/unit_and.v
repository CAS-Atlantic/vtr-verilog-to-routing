
module simple_op(a,b,out);
  input 	[`WIDTH-1:0] a;
  input 	[`WIDTH-1:0] b;
  output 	[`WIDTH-1:0] out;

 and(out,a,b);
  
endmodule

//Bug 