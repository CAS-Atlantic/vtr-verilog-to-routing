module GOOD();
endmodule

module axi();

    parameter j = 6;  
    parameter LOG_MASTER      = 5;

    generate

      if (j == 0 )  begin : LAST_NODE
                         
      end else if ( j < LOG_MASTER - 1)  begin : MIDDLE_NODES // START of MIDDLE LEVELS Nodes
                                  
          end 
          else begin : LEAF_NODES  // START of FIRST LEVEL Nodes (LEAF)
             GOOD good();                      
          end 
    endgenerate

endmodule

