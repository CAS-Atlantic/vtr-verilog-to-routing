module simple_op(a,b,c,d,e,f);
parameter WIDTH = 8,
          SIZE = 6;

input   [WIDTH-1:0] a ;
input   [WIDTH-1:0] b ;
output  [WIDTH-1:0] c ;
input   [SIZE-1:0] d ;
input   [SIZE-1:0] e ;
output  [SIZE-1:0] f ;

assign c = a + b;
assign f = d - e;

endmodule