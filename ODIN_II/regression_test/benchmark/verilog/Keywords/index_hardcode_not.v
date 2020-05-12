`define WIDTH 8

module simple_op(a, out);
    input   [`WIDTH-1:0]a;
    output  [`WIDTH-1:0]out;

    not(out[0], a[0]);
    not(out[1], a[1]);
    not(out[2], a[2]);
    not(out[3], a[3]);
    not(out[4], a[4]);
    not(out[5], a[5]);
    not(out[6], a[6]);
    not(out[7], a[7]);

endmodule