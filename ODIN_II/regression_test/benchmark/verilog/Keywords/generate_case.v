module simple_op(a,b,c);
input a,b;
output c;

parameter en = 2'b00; 

generate
    case(en)
        2'b00   : begin : u1
                    and_1bit u1(a,b,c); 
                  end 
        2'b01   : begin : u2 
                    or_1bit u2(a,b,c);
                  end 
        2'b10   : begin : u3 
                    nand_1bit u3(a,b,c);
                  end  
        default : begin : u4
                    nor_1bit u4(a,b,c);
                  end  
    endcase
endgenerate 

endmodule 


module u1(a,b,c);
input a,b;
output c;

and(c,b,a);

endmodule 

module u2(a,b,c);
input a,b;
output c;

or(c,b,a);

endmodule 

module u3(a,b,c);
input a,b;
output c;

nand(c,b,a);

endmodule 

module u4(a,b,c);
input a,b;
output c;

nor(c,b,a);

endmodule 
