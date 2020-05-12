//Case example 

module simple_op(
    input [2:0] a,b,c,
          [1:0] sel,
    output reg [2:0] out);

always @(a,b,c,sel) begin 
    case(sel)
        2'b00   :   out = a;
        2'b01   :   out = b;
        2'b10   :   out = c;
        default :   out = 0;
    endcase
    end
endmodule


