module top_module
(
    input  [1:0] a, b, c,
    input cin,
    output [1:0] sumout,
    output cout
);

	wire  cout1;
	wire [1:0] sumout1;	

	adder add1 (a, b, cin, sumout1, cout1);
	adder add2 (.a(c), .b(sumout1), .cin(cout1), .sumout(sumout), .cout(cout));

endmodule
