# rv-research

## Structure

- [hardware](./hardware): Contains a RISC-V CPU (PicoRV32) written in Verilog and a thin wrapper around it
- [simulator](./simulator): Contains C++ code used with Verilator to execute code inside a RISC-V "virtual machine"
- [lib](./lib): Contains basic libraries used inside the simulator, such as I/O and basic system calls
- [program](./program): Contains a C program ran inside the simulator, using the libraries

## Setup

1. Install [oss-cad-suite](https://github.com/YosysHQ/oss-cad-suite-build) and [this RISC-V gcc toolchain](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack). Make sure you add their `bin` directories to PATH. Also install `make` and a C compiler (gcc/clang) for your host computer if you don't have one.
2. Run `make` in the root directory.
3. There should be an executable in the `obj_dir` directory (`Vcpu`). Running it will start the simulator, run the program, and exit.

