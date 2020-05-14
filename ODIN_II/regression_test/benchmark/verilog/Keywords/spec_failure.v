module simple_op(A,Y,se);
input A;
input se;
output Y;

specparem T1 = 4;
specparem T2 = 8;

specify 
    if (se)
    (A => Y) = T1;
    if ~(se)
    (A => Y) = T1;
endspecify 

assign Y = A;

endmodule

//suppose to fail, specparem can't be outside specify block