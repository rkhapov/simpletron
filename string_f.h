#ifndef INT_TO16STR_H
#define INT_TO16STR_H

//Функция для преобразования числа в 16-тиричную строку

#include <cstring>
#include <string>

namespace smpl
{
    namespace funcs
    {
        std::string intTo16String(int n);
        int string16ToInt(const std::string &string);

        bool isNumber(const std::string &str);
    }
}

#endif // INT_TO16STR_H

