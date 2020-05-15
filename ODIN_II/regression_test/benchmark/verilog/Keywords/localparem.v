//localparem

// However, a defparam statement in a hierarchy in or under a generate block instance (see 12.4) or an array of
// instances (see 7.1 and 12.1.2) shall not change a parameter value outside that hierarchy

module simple_op(a,b,c);
localparam WIDTH = 8;

input  [WIDTH-1:0] a ;
input  [WIDTH-1:0] b ;
output [WIDTH-1:0] c ;

assign c = a + b;

endmodule 