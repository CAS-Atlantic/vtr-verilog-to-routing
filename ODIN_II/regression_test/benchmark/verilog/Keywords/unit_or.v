//doesn't run on odin but does on verilator

module simple_op(a,b,out);
    input 	    [`WIDTH-1:0] a;
    input 	    [`WIDTH-1:0] b;
    output      [`WIDTH-1:0] out;

or(out,a,b);
endmodule 

//Bug with odin having more than one bit