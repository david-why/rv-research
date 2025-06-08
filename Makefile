RVARCH ?= rv32im
RVABI ?= ilp32
RVGCC ?= riscv-none-elf-
RVLDSCRIPT ?= sim.ld
RVROOT = $(shell whereis -b $(RVGCC)gcc | cut -d : -f 2)
RVLIBPATH = $(shell ls -d `dirname $(RVROOT)`/../*/lib/$(RVARCH)/$(RVABI))
RVGCCPATH = $(shell ls -d `dirname $(RVROOT)`/../lib/gcc/*/*/$(RVARCH)/$(RVABI))

all: obj_dir/Vcpu program/main.bin

run: all
	./obj_dir/Vcpu

clean:
	rm -rf obj_dir
	rm -vf program/*.o program/*.elf program/*.bin
	rm -vf lib/*.o lib/*.a lib/*.gch


obj_dir/Vcpu: hardware/*.v simulator/main.cpp
	CXXFLAGS=-std=c++17 verilator --cc --build --exe --top-module cpu -Ihardware cpu.v simulator/main.cpp

lib/riscv.o: lib/riscv.h

%.o: %.c
	$(RVGCC)g++ -Os -fno-pic -march=$(RVARCH) -mabi=$(RVABI) -fno-stack-protector -w -Wl,--no-relax -c $< -o $@

%.o: %.S
	$(RVGCC)as -march=$(RVARCH) -mabi=$(RVABI) $^ -o $@

lib/libriscv.a: lib/riscv.o
	$(RVGCC)ar rcs lib/libriscv.a lib/riscv.o
	$(RVGCC)ranlib lib/libriscv.a

program/main.elf: program/main.cpp lib/libriscv.a lib/crtrv.o lib/$(RVLDSCRIPT)
	$(RVGCC)g++ -O3 -g -Ilib -fno-pic -march=$(RVARCH) -mabi=$(RVABI) -fno-stack-protector -w -Wl,--no-relax -c program/main.cpp -o program/main.o
	$(RVGCC)ld -m elf32lriscv -b elf32-littleriscv --no-relax --print-memory-usage -Tlib/$(RVLDSCRIPT) \
		program/main.o -o program/main.elf -Llib -lriscv \
		-L$(RVLIBPATH) -lsupc++ -lc -lm \
		-L$(RVGCCPATH) -lgcc 
	$(RVGCC)strip -d program/main.elf

program/main.bin: program/main.elf
	$(RVGCC)objcopy program/main.elf program/main.bin -O binary
	chmod -x program/main.bin
