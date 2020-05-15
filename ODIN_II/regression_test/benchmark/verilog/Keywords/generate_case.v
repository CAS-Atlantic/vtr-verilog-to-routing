

module simple_op(in1,in2,in3,in4,out);
input [1:0] in1,in2,in3,in4;
output [1:0] out;

generate
    case(`en)
        2'b00   : begin
                   assign out = in1; 
                  end 
        2'b01   : begin 
                    assign out = in2; 
                  end 
        2'b10   : begin 
                    assign out = in3; 
                  end  
        default : begin 
                    assign out = in4; 
                  end  
    endcase
endgenerate 

endmodule 


// module u1(a,b,c);
// input a,b;
// output c;

// and(c,b,a);

// endmodule 

// module u2(a,b,c);
// input a,b;
// output c;

// or(c,b,a);

// endmodule 

// module u3(a,b,c);
// input a,b;
// output c;

// nand(c,b,a);

// endmodule 

// module u4(a,b,c);
// input a,b;
// output c;

// nor(c,b,a);

// endmodule 
