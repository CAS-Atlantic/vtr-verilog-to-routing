module simple_op(A,Y,se);
input A;
input se;
output Y;

specify 
    specparem T1 = 4;
    specparem T2 = 8;
    if (se)
    (A => Y) = T1;
    if ~(se)
    (A => Y) = T1;
endspecify 

assign Y = A;

endmodule


//State sensitive

// Online examples with variables as 3:4:5
//Simple path done
//State dependent path done
//Edge sensitive path not done