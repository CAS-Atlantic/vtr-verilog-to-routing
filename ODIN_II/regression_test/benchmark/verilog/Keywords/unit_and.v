`define WIDTH 32

module simple_op(a,b,out);
  input 	a[`WIDTH-1:0];
  input 	b[`WIDTH-1:0];
  output 	out[`WIDTH-1:0];

 and(out,a,b);
  
endmodule