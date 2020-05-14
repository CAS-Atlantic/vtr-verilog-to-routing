//Case example 

module simple_op();
    input [2:0] a,b;
    input sel;
    output reg [2:0] out;

always @(a,b,sel) begin 
    case(sel)
        1'b0   :   out = a;
        1'b1   :   out = b;
    endcase
    end
endmodule


