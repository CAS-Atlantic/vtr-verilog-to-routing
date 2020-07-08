module simple_op(a,b,out1,out2);
    input  a;
    input  b;
    output reg out;

    always @(*) begin
        out1 <= a;
        out2 <= b;
    end
endmodule