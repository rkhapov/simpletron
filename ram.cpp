#include <stdexcept>

#include "ram.h"
#include "string_f.h"

smpl::Ram::Ram(int size)
{
    _size = size;
    _ram = new ubyte[_size];
}

smpl::Ram::~Ram()
{
    delete []_ram;
}

smpl::ubyte &smpl::Ram::operator [](int index)
{
    if (index < 0 || index >= _size)
    {
        throw std::out_of_range("RAM: address " + smpl::funcs::intTo16String(index)
                                + " out of range 0 ... " + smpl::funcs::intTo16String(_size - 1));
    }

    return _ram[index];
}

int smpl::Ram::size() const
{
    return _size;
}


