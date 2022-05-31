#!/usr/bin/env bash
{ # try
   make build_cov.sc sim.sc SUITE=riscv_tests PROG=rsort VERILATOR="$1 -sverilog -parse -nopython -verbose -timescale=1ps/1ps -elabuhdm -d coveruhdm -verbose -lowmem"  && echo "OK"
    #save your output

} || { # catch
    # save log for exception
    echo "OK"
}
