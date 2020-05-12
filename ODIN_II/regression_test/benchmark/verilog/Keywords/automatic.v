module sv_task(a,b,c);
  input [7:0] a;
  input [7:0] b;
  output[7:0] c;
 
  //task to add two integer numbers.
  task automatic sum;
    input [7:0] a;
    input [7:0] b;
    output [7:0] c;
    c = a+b;  
  endtask
 
  initial begin
    sum(a,b,c);
  end
  
endmodule