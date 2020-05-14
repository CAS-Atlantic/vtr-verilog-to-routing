
module simple_op(a,b,out);
  input 	[`WIDTH-1:0] a;
  output 	[`WIDTH-1:0] out;

 and(out,a);
  
endmodule

//Bug 