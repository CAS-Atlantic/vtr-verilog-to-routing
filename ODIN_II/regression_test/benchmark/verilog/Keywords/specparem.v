module simple_op(A,Y);
input A;
output Y;

specify 
    specparem T = 4;
    (A => Y) = T;
endspecify 

assign Y = A;

endmodule 

//Simple path 