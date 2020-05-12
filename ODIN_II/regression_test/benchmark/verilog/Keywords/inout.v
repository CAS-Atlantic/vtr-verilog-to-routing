module simple_op (oe, clk, inp, outp, bidir);

// Port Declaration

input   oe;
input   clk;
input   [7:0] inp;
output  [7:0] outp;
inout   [7:0] bidir;

reg     [7:0] a;
reg     [7:0] b;

assign bidir = oe ? a : 8'bZ ;
assign outp  = b;

// Always Construct

always @ (posedge clk)
begin
    b <= bidir;
    a <= inp;
end

endmodule

//example taken from intel
