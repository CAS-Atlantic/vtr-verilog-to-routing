module t (/*AUTOARG*/);

  integer a, b;

  initial begin
     for (; ; ) ;
     for (; ; a=a+1) ;
     for (; ; a=a+1, b=b+1) ;
     for (; a<1; ) ;
     for (; a<1; a=a+1) ;
     for (; a<1; a=a+1, b=b+1) ;
     for (a=0; a<1; ) ;
     for (a=0; a<1; a=a+1) ;
     for (a=0; a<1; a=a+1, b=b+1) ;
     for (integer a=0; a<1; ) ;
     for (integer a=0; a<1; a=a+1) ;
     for (integer a=0; a<1; a=a+1, b=b+1) ;
     for (var integer a=0; a<1; ) ;
     for (var integer a=0; a<1; a=a+1) ;
     for (var integer a=0; a<1; a=a+1, b=b+1) ;
     for (integer a=0, integer b=0; a<1; ) ;
     for (integer a=0, integer b=0; a<1; a=a+1) ;
     for (integer a=0, integer b=0; a<1; a=a+1, b=b+1) ;
     $write("*-* All Finished *-*\n");
     $finish;
  end

endmodule


