// verilator example

#include "Vcpu.h"

#define RAM_START 0x00000000
#define ROM_START 0x00800000
#define ACC_START 0x00600000

#define IO_BASE 0x01000000
#define IO_UART0 (IO_BASE + 0x1000)
#define IO_EXIT (IO_BASE + 0x2000)
#define IO_MEMREADS (IO_BASE + 0x3000)

uint8_t mem[0x2000000];

static Vcpu *top;

static void step(int n = 1)
{
    for (int i = 0; i < n; i++)
    {
        top->clk = 0;
        top->eval();
        top->clk = 1;
        top->eval();
    }
}

int main(int argc, char **argv, char **env)
{
    Verilated::commandArgs(argc, argv);
    top = new Vcpu;

    // Initialize memory
    FILE *fp = fopen("program/main.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }
    fread(mem + ROM_START, 1, sizeof(mem) - ROM_START, fp);
    fclose(fp);

    // Initialize simulation inputs
    top->clk = 0;
    top->resetn = 0;
    step();
    top->resetn = 1; // Release reset
    step();

    uint32_t *ramreads = (uint32_t *)(mem + IO_MEMREADS);

    for (int i = 0; i < 200000000; i++)
    {
        top->clk = 1; // Rising edge
        top->eval();
        // printf("Cycle %d: clk = %d, mem = %d\n", i, top->clk, top->mem_instr);
        // print all memory pins, single line
        top->clk = 0; // Falling edge
        top->eval();
        // printf("Cycle %d: clk = %d, mem = %d\n", i, top->clk, top->mem_instr);
        // printf("trap = %d, mem_valid = %d, mem_instr = %d, mem_ready = %d, mem_wstrb = %d, mem_addr = %d, mem_wdata = %d, mem_rdata = %d\n", top->trap, top->mem_valid, top->mem_instr, top->mem_ready, top->mem_wstrb, top->mem_addr, top->mem_wdata, top->mem_rdata);
        if (top->mem_valid)
        {
            // Read/write data
            uint32_t addr = top->mem_addr;
            if (addr > sizeof(mem))
            {
                printf("Error: address out of range: 0x%08x\n", addr);
                break;
            }
            if (top->mem_wstrb)
            {
                // Write data
                uint32_t data = top->mem_wdata;
                uint32_t mask = 0;
                for (int j = 0; j < 4; j++)
                    if (top->mem_wstrb & (1 << j))
                        mask |= (0xFF << (j * 8));
                uint32_t old_data = *(uint32_t *)(mem + addr);
                *(uint32_t *)(mem + addr) = (old_data & ~mask) | (data & mask);
                if (addr == IO_UART0)
                {
                    // UART output
                    putchar(data & 0xFF);
                }
                else if (addr == IO_EXIT)
                {
                    // Exit simulation
                    printf("Exit with status %u\n", data);
                    break;
                }
            }
            else
            {
                // Read data
                uint32_t data = *(uint32_t *)(mem + top->mem_addr);
                top->mem_rdata = data;
                if (top->mem_addr >= RAM_START && top->mem_addr < ROM_START)
                {
                    (*ramreads)++;
                }
                // printf("Read data from 0x%08x: 0x%08x\n", addr, data);
            }
            top->mem_ready = 1;
        }
        else
        {
            top->mem_ready = 0;
        }
    }

    // Clean up
    delete top;
    return 0;
}
