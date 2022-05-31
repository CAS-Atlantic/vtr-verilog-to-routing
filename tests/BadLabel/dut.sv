package pack;
endpackage : bad_pack

class c;
endclass : bad_c

interface intf;
endinterface : bad_int


/* Test attribute on interface */
(* test_interface *) interface itf;
 
  (* test_interface2 *) interface itf2;
  endinterface : itf2;

  (* test_interface2 *) interface itf3;
  endinterface : bad_itf3;

endinterface


module test ();


   // error counter
   bit err = 0;

   initial
   begin : dummy_label
      if (!err) $display("PASSED");
   end : dummy_label_bad

   module sub();
   endmodule : sub

   module sub2();
   endmodule : bad_sub2

initial begin
   
   fork : n1
     print("");
   join : n2 

   fork 
     print("");
   join : n2 

  fork : n1
    print("");
  join 

  end


endmodule : bad_test


module block_tb ();
	reg a = 0;
	initial
	        name1: fork
			a = 1;
		join: name1
endmodule
