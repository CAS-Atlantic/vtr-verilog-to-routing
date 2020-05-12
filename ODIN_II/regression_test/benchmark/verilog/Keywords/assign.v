//assign 
module simple_op(input x,input y,output z,output m);
assign z = x & y;
assign m = x || y;
endmodule 