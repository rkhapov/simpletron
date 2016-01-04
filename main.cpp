#include <iostream>
#include <fstream>
#include <string>

#include "cpu.h"
#include "ram.h"
#include "fileparser.h"

int main(int ac, char **av)
{

    if (ac == 1)
    {
        std::cout << "Expected filename" << std::endl;
        return 0;
    }

    std::string filename = av[1];

    smpl::FileParser fp;
    fp.open(filename);

    smpl::Ram ram(1024 * 1024);

    if (!fp.parse(ram, std::cout))
    {
        return 0;
    }

    //Создаём файл .smplbin
    if (filename.size() > 8)
    {
        if (filename.substr(filename.size() - 8) != ".smplbin")
        {
            filename += ".smplbin";

            std::ofstream smplfile(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

            if (smplfile.is_open())
            {
                smplfile.write(reinterpret_cast<const char*>(&ram[0]), ram.size());
            }
        }
    }
    else
    {
        filename += ".smplbin";

        std::ofstream smplfile(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

        if (smplfile.is_open())
        {
            smplfile.write(reinterpret_cast<const char*>(&ram[0]), ram.size());
        }
    }

    smpl::Cpu cpu;

    try
    {
        while (!cpu.getHalt())
        {
//            std::cout << "Cpu: \n";
//            std::cout << "ip: " << cpu.getIp() << std::endl;
//            std::cout << "acc1 " << (int)cpu.getAcc1() << std::endl;
//            std::cout << "acc2 " << (int)cpu.getAcc2() << std::endl;
            cpu.execute(ram);
        }
    }
    catch (std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
    }

    return 0;
}

