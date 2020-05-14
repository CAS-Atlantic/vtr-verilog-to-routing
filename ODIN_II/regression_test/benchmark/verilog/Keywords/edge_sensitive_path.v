module simple_op(clk,en,q,d);
input clk;
input en;
input d;
output q;
reg temp;


specify 
    (clk *> (q +: d)) = (10,5);
endspecify 

endmodule

//doesn't work yet
