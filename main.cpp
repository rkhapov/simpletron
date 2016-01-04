#include <iostream>
#include <fstream>
#include <string>

#include "cpu.h"
#include "ram.h"
#include "fileparser.h"

int main()
{

    smpl::Ram ram(10);
    smpl::FileParser fp;
    fp.open("1.smplbin");
    fp.parse(ram, std::cout);

    smpl::Cpu cpu;

    while (!cpu.getHalt())
    {
        cpu.execute(ram);
    }

    return 0;
}

