//
// Created by Casie Carl on 12/20/20.
//

#ifndef SERV_PARSER_UTILS_HPP
#define SERV_PARSER_UTILS_HPP
#include <iostream>
#include <list>
#include <sstream>

int                     isDigit(const std::string &value);
std::list<std::string>      split(const std::string &directive);

#endif //SERV_PARSER_UTILS_HPP
