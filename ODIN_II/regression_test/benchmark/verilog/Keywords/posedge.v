module simple_op(clk,res,q,clk_out);

    input res;
    input clk;
    output clk_out;
    output q;
    reg qt;

    always @(posedge clk)
    if(~res)begin
        qt<=~qt;
    end 

    else begin
        qt<=1;
     end

    assign q = qt;
    assign clk_out = clk;

endmodule
