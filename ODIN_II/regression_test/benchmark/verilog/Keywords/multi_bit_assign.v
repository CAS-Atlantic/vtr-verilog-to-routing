
//assign 
module simple_op(x,y,z,m);
input [`WIDTH-1:0] x;
input [`WIDTH-1:0] y;
output [`WIDTH-1:0] z;
output [`WIDTH-1:0] m;

assign z = x;
assign m = y;
endmodule 

