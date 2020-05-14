
module simple_op(c);

parameter b = 1, a = 0;
output c;

generate 
    if (a & b) begin: mult
        u1 u1(c);
    end
    else begin: mult
        u2 u2(c);
    end
endgenerate
  
endmodule 

module u1(c);
output c;

assign c = 1'b1;

endmodule 

module u2(c);
output c;

assign c = 1'b0;

endmodule 