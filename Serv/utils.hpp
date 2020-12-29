//
// Created by Casie Carl on 12/25/20.
//

#ifndef SERV_UTILS_HPP
#define SERV_UTILS_HPP

#include <sys/stat.h>
#include <cstdio>
#include <string>

struct      input
{
    std::string conf;
    std::string interptretator;
    std::string scrypt;
};

long  findFileSize(int fd);

#endif //SERV_UTILS_HPP
