#include <cctype>

#include "fileparser.h"
#include "string_f.h"
#include "cpu.h"

void smpl::FileParser::_cut(const std::string &source, const std::string &delim, std::vector<std::string> &out)
{
    char *buff = new char[source.size() + 1];
    std::strcpy(buff, source.c_str());

    char *token = std::strtok(buff, delim.c_str());

    while (token != nullptr)
    {
        out.push_back(token);;
        token = std::strtok(nullptr, delim.c_str());
    }

    delete []buff;
}

void smpl::FileParser::_toLowerCase(std::string &str)
{
    for (std::size_t i = 0; i < str.size(); i++)
        str[i] = std::tolower(str[i]);
}

int smpl::FileParser::_getTokenType(const std::string &token)
{
    //Если первый символ - буква, то достаточно проверить на совпадение имени по всем инструкциям
    if (std::isalpha(token[0]))
    {
        for (int i = 0; i < Cpu::INSTRUCTION_NUMBER; i++)
        {
            if (token == Cpu::instructions[i].name)
            {
                return INSTRUCTION;
            }
        }
        return UNKNOWN;
    }


    if (token.size() > 1)
    {
        if (token[0] == '#')
        {
            if (funcs::isNumber(token.substr(1)))
                return NUMBER;
        }
        else if(token[0] == '%')
        {
            if (funcs::isNumber(token.substr(1)))
                return ADDRESS;
        }
    }

    return UNKNOWN;
}

bool smpl::FileParser::open(const std::string &filename)
{
    if (filename.size() > 8)
    {
        if (filename.substr(filename.size() - 8) == ".smplbin")
        {
            _isBinaryFile = true;
            _file.open(filename.c_str(), std::ios::in | std::ios::binary);
            return _file.is_open();
        }
    }

    _file.open(filename.c_str());
    return _file.is_open();
}

bool smpl::FileParser::parse(Ram &ram, std::ostream &log)
{
    if (!_file.is_open())
    {
        log << "File not open" << std::endl;
        return false;
    }

    if (_isBinaryFile)
    {
        _smpbinParse(ram, log);
        return true;
    }

    return _parseSourceFile(ram, log);
}


void smpl::FileParser::_smpbinParse(Ram &ram, std::ostream &log)
{
    log << "Loading file..." << std::endl;

    _file.seekg(0);

    ubyte byte;
    int i = 0;
    while (_file.read(reinterpret_cast<char*>(&byte), 1) && i < ram.size())
    {
        ram[i] = byte;
        i++;
    }

    log << "Reading file is finished" << std::endl;
}


bool smpl::FileParser::_parseSourceFile(Ram &ram, std::ostream &log)
{
    log << "Parsing file..." << std::endl;

    _file.seekg(0);

    unsigned int line = 0;
    uword addr = 0;

    while (!_file.eof())
    {
        std::string str;
        std::getline(_file, str);

        //Обрезаем комментарий
        str = str.substr(0, str.find('!'));

        //Переводим в нижний регистр
        _toLowerCase(str);

        //Разбиваем на токены
        std::vector<std::string> tokens;
        _cut(str, "\t ", tokens);

        if (tokens.size() == 0)
        {
            //Ничего не делаем. Строка пустая, либо содержит только комментарий
        }
        else if (tokens.size() == 1 && _getTokenType(tokens[0]) == NUMBER)
        {
            ram[addr] = funcs::string16ToInt(tokens[0].substr(1));
            addr++;
        }
        else if (tokens.size() == 1 && _getTokenType(tokens[0]) == INSTRUCTION)
        {
            for (int i = 0; i < Cpu::INSTRUCTION_NUMBER; i++)
            {
                if (tokens[0] == Cpu::instructions[i].name)
                {
                    ram[addr] = Cpu::instructions[i].code;
                    addr += Cpu::instructions[i].size;
                }
            }
        }
        else if (tokens.size() == 1 && (_getTokenType(tokens[0]) == ADDRESS ||  _getTokenType(tokens[0]) == UNKNOWN))
        {
            log << "Expected instruction or constant, but taken " << tokens[0] << " at line " << line << std::endl;
            return false;
        }
        else if (tokens.size() == 2)
        {
            if (_getTokenType(tokens[0]) == INSTRUCTION && _getTokenType(tokens[1]) == NUMBER)
            {
                for (int i = 0; i < Cpu::INSTRUCTION_NUMBER; i++)
                {
                    if (tokens[0] == Cpu::instructions[i].name)
                    {
                        ram[addr] = Cpu::instructions[i].code;
                        ram[addr + 1] = funcs::string16ToInt(tokens[1].substr(1));
                        addr += Cpu::instructions[i].size;
                    }
                }
            }
            else if (_getTokenType(tokens[0]) == INSTRUCTION && _getTokenType(tokens[1]) == ADDRESS)
            {
                for (int i = 0; i < Cpu::INSTRUCTION_NUMBER; i++)
                {
                    if (tokens[0] == Cpu::instructions[i].name)
                    {
                        ram[addr] = Cpu::instructions[i].code;
                        *(reinterpret_cast<uword*>(&ram[addr + 1])) = funcs::string16ToInt(tokens[1].substr(1));
                        addr += Cpu::instructions[i].size;
                    }
                }
            }
            else
            {
                log << "Unknown instructions or tokens at line " << line << std::endl;
                return false;
            }
        }
        else
        {
            log << "Too many tokens at line " << line << std::endl;
            return false;
        }

        line++;
    }

    log << "Parse ended" << std::endl;

    return true;
}
