#include <iostream>

#include "cpu.h"

int main()
{
    //Программа для сложения чисел записанная в машинных кодах :)
    smpl::Ram ram(1024 * 1024);
    ram[0] = 1;
    ram[1] = 3;
    ram[2] = 1;
    ram[3] = 12;
    ram[4] = 2;
    ram[5] = 0;


    smpl::Cpu cpu;

    while (!cpu.getHalt())
    {
        cpu.nextInstruction(ram);
    }

    return 0;
}

