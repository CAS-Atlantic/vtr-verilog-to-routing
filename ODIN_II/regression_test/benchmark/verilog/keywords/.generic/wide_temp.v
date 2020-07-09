`define RANGE [2:0]

module simple_op(a,b,out);
    input 	`RANGE a;
    input   `RANGE b;
    output 	`RANGE out;

    xor(out[0],a[0],b[0]);
    xor(out[1],a[1],b[1]);
	xor(out[2],a[2],b[2]);
endmodule 