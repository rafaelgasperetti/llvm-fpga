Source folder of HLS project.
In order to be able to compile the whole project is necessary to run the 'llvm-fpga.sh' shell scripts that solves all the dependency libraries, clones the project and compiles the project and its dependencies.

The HLS exclusive project is configured at the 'hls' folder.
It's, for now, auto compiled end executed by running the 'hls.sh' shell scripts, but needs the 'llvm-fpga.sh' file to run it all first.
The whole project three must be in the same file hierarchy for the includes to work well.

The compilation is not yet running because of the migration from manual VHDL generation to automatic VHDL generation and it's commands dependencies.