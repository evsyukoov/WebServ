//
// Created by Casie Carl on 12/25/20.
//

#ifndef SERV_UTILS_HPP
#define SERV_UTILS_HPP

#include <sys/stat.h>
#include <cstdio>
#include <string>
#include <map>
#include <netinet/in.h>
#include <iostream>
#include "File.hpp"

struct      input
{
    std::string conf;
    std::string interptretator;
    std::string scrypt;
    std::string remote_addr;
    std::map<std::string, std::string> *reqestMap;
    std::string root;
};

long  findFileSize(int fd);
size_t  ft_strlen(char *str);
char   *ft_strdup(char *str);

#endif //SERV_UTILS_HPP
