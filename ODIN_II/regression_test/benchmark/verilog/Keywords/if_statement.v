module simple_op(input en,input d, output reg q);

always @(en or d)
    if (en)
        q = d;
    else 
        q = 1;
endmodule 