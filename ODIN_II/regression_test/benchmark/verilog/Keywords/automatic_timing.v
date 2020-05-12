module simple_op(d1,q1,clk,res1,d2,res2);
 input res;
    input clk;
    output q;
    reg qt;

task count;
    input res;
    input clk;
    output q;
    reg qt;

    begin 
    always@(posedge clk)
        if(~res)begin
        qt<=~qt;
        end 

    else begin
        qt<=1;
     end

    assign q = qt;
    end
endtask

initial begin
    count(res,clk,q);
end 

endmodule 


