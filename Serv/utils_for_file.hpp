//
// Created by Semen Jejgin on 12/26/20.
//

#ifndef SERV_UTILS_FOR_FILE_HPP
#define SERV_UTILS_FOR_FILE_HPP

#include <iostream>
#include <vector>

bool						predicate(char c);
void 						trimmer(std::string &str);
std::vector<std::string>	ft_split(std::string str, std::string delimetr);
bool						charsetPredicate(char c);
bool						acceptPredicete(char c);

#endif //SERV_UTILS_FOR_FILE_HPP
