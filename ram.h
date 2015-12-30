#ifndef RAM_H
#define RAM_H

namespace smpl
{
    typedef unsigned char ubyte;

    class Ram
    {
    public:
        Ram(int size);
        ~Ram();

        int size() const;

        ubyte &operator[] (int index);

    private:
        ubyte *_ram;
        int _size;
    };
}

#endif // RAM_H
