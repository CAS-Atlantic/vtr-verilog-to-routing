
//assign 
module simple_op(x,y,z,m);
input x;
input y;
output z;
output m;

assign z = x & y;
assign m = x || y;
endmodule 