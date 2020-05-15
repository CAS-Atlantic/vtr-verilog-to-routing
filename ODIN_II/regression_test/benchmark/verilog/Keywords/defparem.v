//defparem
// Using the defparam statement, parameter values can be changed in any module instance throughout the
// design using the hierarchical name of the parameter. See 12.5 for hierarchical names.
// However, a defparam statement in a hierarchy in or under a generate block instance (see 12.4) or an array of
// instances (see 7.1 and 12.1.2) shall not change a parameter value outside that hierarchy

module top(clk,in1,in2,out1,out2);
    parameter 
    size = 8,
    sizetwo = 6;
    input clk;
    input [size-1:0] in1;
    input [sizetwo-1:0] in2;
    output [size-1:0] out1; 
    output [sizetwo-1:0] out2; 

    assign out1 = in1;
    assign out2 = in2;
    
endmodule

module annotate;
defparam
top.size = 5,
top.sizetwo = 10;
endmodule

// Example structure taken from IEEE Standard for Verilog® Hardware Description Language