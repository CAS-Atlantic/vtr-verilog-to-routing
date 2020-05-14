`define WIDTH 8

module simple_op(c);
  output[`WIDTH-1:0] c;
 
 
  task automatic assgn;
    output [`WIDTH-1:0] o;
    assign o = 8'b10101010;
  endtask
 
  initial begin
    assgn(c);
  end
  
endmodule

//fails on odin 

// Welcome to ODIN II version 0.1 - the better High level synthesis tools++ targetting FPGAs (mainly VPR)
// Email: jamieson.peter@gmail.com and ken@unb.ca for support issues

// Using Lut input width of: -1
// Verilog: automatic.v
// --------------------------------------------------------------------
// High-level synthesis Begin
// Parser starting - we'll create an abstract syntax tree. Note this tree can be viewed using Grap Viz (see documentation)
// Adding file regression_test/benchmark/verilog/Keywords/automatic.v to parse list
// Warning::AST automatic.v::10 ODIN II does not (yet) differentiate between automatic and static tasks & functions. IGNORING
//   endtask
// Optimizing module by AST based optimizations
// Converting AST into a Netlist. Note this netlist can be viewed using GraphViz (see documentation)
// ==========================
// Detected Top Level Module: 	simple_op
// ==========================
// Segmentation fault (core dumped)
