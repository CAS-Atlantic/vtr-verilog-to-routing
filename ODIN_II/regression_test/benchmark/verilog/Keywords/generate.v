module test(out);

  reg       [7:0] i = 8'b10101010;
  wire      [7:0] o;
  output    [7:0] out;

  // Generate Block, Verilog-2001
  genvar index;
  generate
    for (index = 0; index < 8;
      index = index + 1) begin
      buffer_not NBUF(
        .in(i[index]),
        .out(o[index]));
    end
  endgenerate
  assign out = o;
endmodule

module buffer_not(in, out);
  input in;
  output out;
  
  assign out = ~in;

endmodule

//example taken from playground 
//error Error: Could not hook up the pin test^BUF~16: not available.
//not producing the input vectors at all and finding error