#ifndef FFILEPARSER_H
#define FFILEPARSER_H

//FileParser занимается преобразованием файла в машинные коды. Может преобразовать как любой файл
//с исходным кодом на ассемблере, так и .smplbin

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ram.h"

namespace smpl
{
    class FileParser
    {
    public:
        bool open(const std::string &filename);

        //Парсинг происходит прямо в память
        bool parse(Ram &ram, std::ostream &log);

    public:
        std::ifstream _file;
        bool _isBinaryFile;

        //Чтение файла .smplbin
        void _smpbinParse(Ram &ram, std::ostream &log);

        //Парсинг файла с исходным кодом
        bool _parseSourceFile(Ram &ram, std::ostream &log);

        //Функции для парсинга

        //Разбивание строки по символам-делителям
        void _cut(const std::string &source, const std::string &delim, std::vector<std::string> &out);

        //Преобразование строки в нижний регистр
        void _toLowerCase(std::string &str);

        //Получение типа токена
        //Токен должен быть без пробельных символов
        enum { NUMBER, ADDRESS, INSTRUCTION, UNKNOWN };
        int _getTokenType(const std::string &token);
    };
}

#endif // FFILEPARSER_H
