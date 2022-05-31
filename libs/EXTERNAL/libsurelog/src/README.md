# SURELOG project
## Executable: surelog

## Dependancies Install 

Please see [`INSTALL`](../INSTALL.md)

## Build
 
```bash
cd Surelog

If you had a previous install, remove it first:
  make uninstall  (PREFIX=...)
  
make
or 
make debug

make install (/usr/local/bin and /usr/local/lib/surelog by default, use PREFIX=<path> for alternative locations)

To make with Python you have to add the option to build/cmake:  -DSURELOG_WITH_PYTHON=1
```

## Run a test

* When in the Surelog/build directory, run one of the following:

* bin/surelog -help

* bin/surelog -writepp -parse ../tests/UnitElabBlock/top.v

* bin/hellosureworld ../tests/UnitElabBlock/top.v -parse -mutestdout

* bin/hellouhdm ../tests/OneFF/dut.v -d uhdm -d coveruhdm -elabuhdm

## Create your own executable with the libsurelog.a library

* The CMake file [`CMakeLists.txt`](../tests/TestInstall/CMakeLists.txt) contains the instructions to create your own executable that uses the surelog library for design parsing and creation of the Design/Testbench data model.

* The source file [`hellosureworld.cpp`](hellosureworld.cpp) contains the "HelloWorld" for surelog library usage and Surelog data model browsing. 

* The source file [`hellouhdm.cpp`](hellouhdm.cpp) contains the "HelloWorld" for surelog library usage and UHDM VPI data model browsing.

## Modus operanti for grammar development:

* Edit the grammar file in the G4 directory, test the grammar locally with the java targets: 
  * cd Surelog/grammar;
  * Edit the following section in SV3_1aLexer.g4 for the C++ (Surelog build) or Java target (Java grammar development):
```
@lexer::members {
// C++ target:
   bool sverilog;
// Java target:
// Boolean sverilog = true;
}
```

  * ant compile_java;
  * ant javac;
  * ant test_pp_tokens or
  * ant test_tree ... (There are several debug targets in the build.xml)

## TESTS and REGRESSIONS

The following regression script will run all tests under tests and third_party/tests:
* cd build
* ../tests/regression.tcl 

Regression options:
* regression.tcl help   
* regression.tcl tests=<testname>     (Tests matching regular expression)
  * test=<testname>                   (Just that test)
  * debug=<none, valgrind, ddd>
  * build=<debug, release>
  * mt=<nbThreads>                    (Number of threads per process -
                                       regression runs 1 process at a time)
  * mp=<nbProcesses>                  (Number of processes per test -
                                       regression runs 1 process at a time)			       
  * large                             (large tests too)
  * show_diff                         (Shows text diff)
* regression.tcl update [test=<testname>] (Updates the golden files)  

## PROFILING / MEMORY ANALYSIS

* Either
   * env CPUPROFILE=./prof env LD_PRELOAD="/usr/lib/libprofiler.so"  build/dist/Debug/surelog <test>
* Or 
   * google-pprof --callgrind  build/dist/Debug/surelog prof >> call
   * kcachegrind call 

* Get Google tools: 
   * sudo apt-get install google-perftools graphviz
   * sudo apt-get install libgoogle-perftools-dev
   * sudo apt-get install gperftools

* When running valgrind add the -nopython command line argument

## SOURCE FORMATTING

 * When submitting a source code change for review, please format your code using:
 * clang-format -i -style=Google -sort-includes=false <files>

## Useful links:

http://ecee.colorado.edu/~mathys/ecen2350/IntelSoftware/pdf/IEEE_Std1800-2017_8299595.pdf
https://google.github.io/flatbuffers/flatbuffers_guide_use_cpp.html
https://www.csee.umbc.edu/portal/help/VHDL/verilog/command_line_plus_options.html
https://www.edaplayground.com/

