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
std::vector<std::string>      split2(const std::string &s, char delim);
int error(std::string msg);
int                 isUnexpectedSymbol(const std::string &value, char sym);
int                 isErrorDelimetr(const std::string &directive);
int                 skipSpaces(const std::string &str);

#endif //SERV_PARSER_UTILS_HPP
