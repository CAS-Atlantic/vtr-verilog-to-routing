module simple_op(input en,input d, output reg q);

always @(en or d) begin
    assign q = en;
    if (en)
        q = d;
    end
endmodule 