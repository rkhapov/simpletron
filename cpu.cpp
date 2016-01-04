#include <iostream>

#include "cpu.h"
#include "string_f.h"

const smpl::Cpu::Instruction smpl::Cpu::instructions[]  = {
    { "halt",  0, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_halt  },
    { "read",  1, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_read  },
    { "write", 2, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_write },

    { "mov12", 3, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_mov12 },
    { "mov21", 4, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_mov21 },
    { "movc1", 5, 2, smpl::Cpu::Instruction::NUMBER,   smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_movc1 },
    { "movc2", 6, 2, smpl::Cpu::Instruction::NUMBER,   smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_movc2 },
    { "load",  7, 3, smpl::Cpu::Instruction::ADDRESS,  smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_load  },
    { "store", 8, 3, smpl::Cpu::Instruction::ADDRESS,  smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_store },

    { "jmp",   9,  3, smpl::Cpu::Instruction::ADDRESS,  smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_jmp  },
    { "je",    10, 3, smpl::Cpu::Instruction::ADDRESS, smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_je    },
    { "jl",    11, 3, smpl::Cpu::Instruction::ADDRESS, smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_jl    },

    { "add",   12, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_add  },
    { "sub",   13, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_sub  },
    { "mul",   14, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_mul  },
    { "div",   15, 1, smpl::Cpu::Instruction::NONE,     smpl::Cpu::Instruction::NONE,    &smpl::Cpu::_div  }

};

//Макрос для неиспользуемой переменной
#define __UNUSED_VARIABLE__(a) do { /*пусто*/ } while (&a + 1 == 0);

void smpl::Cpu::_halt(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _haltFlag = true;
}

void smpl::Cpu::_read(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    int t;
    std::cout << "$";
    std::cin >> std::hex >> t;
    _acc1 = t;
}

void smpl::Cpu::_write(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    std::cout << ">" << std::hex << (unsigned int)_acc1 << std::endl;
}

void smpl::Cpu::_mov12(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _acc2 = _acc1;
}

void smpl::Cpu::_mov21(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _acc1 = _acc2;
}

void smpl::Cpu::_movc1(Ram &ram)
{
    _acc1 = ram[_ip + 1];
}


void smpl::Cpu::_movc2(Ram &ram)
{
    _acc2 = ram[_ip + 1];
}

void smpl::Cpu::_load(Ram &ram)
{
    if ((int)_ip + 2 >= ram.size())
        throw CpuError("Bad attempt to get address to load instruction at " + funcs::intTo16String(_ip));

    _acc1 = ram[*(reinterpret_cast<uword*>(&ram[_ip + 1]))];
}

void smpl::Cpu::_store(Ram &ram)
{
    if ((int)_ip + 2 >= ram.size())
        throw CpuError("Bad attempt to get address to store instruction at " + funcs::intTo16String(_ip));

    ram[*(reinterpret_cast<uword*>(&ram[_ip + 1]))] = _acc1;
}

void smpl::Cpu::_jmp(Ram &ram)
{
    if ((int)_ip + 2 >= ram.size())
        throw CpuError("Bad attempt to get address to jmp instruction at " + funcs::intTo16String(_ip));

    _ip = *(reinterpret_cast<uword*>(&ram[_ip + 1])) - 3;
}

void smpl::Cpu::_je(Ram &ram)
{
    if (_acc1 == _acc2)
        _jmp(ram);
}

void smpl::Cpu::_jl(Ram &ram)
{
    if (_acc1 < _acc2)
        _jmp(ram);
}

void smpl::Cpu::_add(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _acc1 += _acc2;
}

void smpl::Cpu::_sub(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _acc1 -= _acc2;
}


void smpl::Cpu::_mul(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);
    _acc1 *= _acc2;
}


void smpl::Cpu::_div(Ram &ram)
{
    __UNUSED_VARIABLE__(ram);

    if (_acc2 == 0)
        throw std::runtime_error("Division by zero at " + funcs::intTo16String(_ip));

    _acc1 /= _acc2;
}

void smpl::Cpu::execute(Ram &ram)
{
    for (ubyte i = 0; i < INSTRUCTION_NUMBER; i++)
    {
        if (ram[_ip] == instructions[i].code)
        {
            (this->*instructions[i].function)(ram);
            if (instructions[i].code < 9 || instructions[i].code > 11)
                _ip += instructions[i].size;
            return;
        }
    }

    throw CpuError("Unknown instruction " + funcs::intTo16String(ram[_ip]) + " at address " + funcs::intTo16String(_ip));
}
