#ifndef CPU_H
#define CPU_H

#include <string>
#include <stdexcept>

#include "ram.h"

namespace smpl
{
    typedef unsigned short uword;

    class CpuError: public std::runtime_error
    {
    public:
        CpuError(const std::string &msg):
            runtime_error(msg)
        {}
    };

    class Cpu
    {

    public:
        Cpu(): _ip(0), _haltFlag(false) {}

        void resetIp() { _ip = 0; }
        void resetHalt() { _haltFlag = false; }

        ubyte getAcc1() const { return _acc1; }
        ubyte getAcc2() const { return _acc2; }
        unsigned int getIp() const { return _ip; }
        bool getHalt() const { return _haltFlag; }

        //Интсрукции
        void _halt(Ram &ram);
        void _read(Ram &ram);
        void _write(Ram &ram);

        void _mov12(Ram &ram);
        void _mov21(Ram &ram);
        void _movc1(Ram &ram);
        void _movc2(Ram &ram);
        void _load(Ram &ram);
        void _store(Ram &ram);

        void _jmp(Ram &ram);
        void _je(Ram &ram);
        void _jl(Ram &ram);

        void _add(Ram &ram);
        void _sub(Ram &ram);
        void _mul(Ram &ram);
        void _div(Ram &ram);

        struct Instruction
        {
            std::string name;
            ubyte       code;
            ubyte       size;

            //Тип параметров
            enum { NONE, ADDRESS, NUMBER, REGISTR };

            int arg1;
            int arg2;

            //Каждая инструкция будет хранить указатель на функцию, которая занимается выполнением инструкции
            void (Cpu::*function)(Ram &ram);

            bool operator==(const Instruction &instr) const
            {
                return (name == instr.name) && (arg1 == instr.arg1) && (arg2 == instr.arg2);
            }
        };

        //Все инструкции данного процессора
        static const int INSTRUCTION_NUMBER = 16;
        static const Instruction instructions[INSTRUCTION_NUMBER];

        //Выполнение следующей инструкции
        void execute(Ram &ram);

    private:

        ubyte _acc1;
        ubyte _acc2;

        uword _ip;

        bool _haltFlag;
    };
}

#endif // CPU_H
