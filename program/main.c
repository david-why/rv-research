#include "riscv.h"

int main()
{
    printf("Hello, RISC-V!\n");
    printf("Current cycle count: %d\n", (int)getcycles());
    printf("Current cycle count again: %d\n", (int)getcycles());
    return 0;
}
