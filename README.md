# rv-research

## Structure

- [hardware](./hardware): Contains a RISC-V CPU (PicoRV32) written in Verilog and a thin wrapper around it
- [simulator](./simulator): Contains C++ code used with Verilator to execute code inside a RISC-V "virtual machine"
- [lib](./lib): Contains basic libraries used inside the simulator, such as I/O and basic system calls
- [program](./program): Contains a C program ran inside the simulator, using the libraries

