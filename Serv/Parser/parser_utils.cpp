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
