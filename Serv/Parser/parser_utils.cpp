//
// Created by Casie Carl on 12/20/20.
//
#include "parser_utils.hpp"

std::list<std::string>      split(const std::string &directive)
{
    std::list<std::string>  dir;
    std::istringstream iss(directive, std::istringstream::in);
    std::string       word;
    while (iss >> word)
        dir.push_back(word);
    return (dir);

}

int                     isDigit(const std::string &value)
{
    for (int i = 0; i < value.size();i++)
    {
        if (!std::isdigit(value[i]))
            return (0);
    }
    return (1);
}

int error(std::string msg) {
    std::cout << "Config error: " << msg << std::endl;
    return (-1);
}

int                 isUnexpectedSymbol(const std::string &value, char sym)
{
    for(int i = 0; i < value.size(); i++)
    {
        if (value[i] == sym)
            return (1);
    }
    return (0);
}
