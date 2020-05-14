//specify

module simple_op(A,Y);
input A;
output Y;

specify 
    (A => Y) = (4);
endspecify 

assign Y = A;

endmodule 
