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
#include "unistd.h"
#define PRINT(x) { std::cerr << "\033[32m" << x << "\033[0m" << std::endl; }
#define BUFFER_SIZE 100

struct      input
{
    std::string conf;
    std::string interptretator;
    std::string scrypt;
    std::string remote_addr;
    std::string root;
    std::map<std::string, std::string> *requestMap;
};

long    findFileSize(int fd);
void    inet_toip4(void *sAddr, std::string &buffer);

std::string     headerPairToStr(std::pair<std::string, std::string> const &pair);
std::string     headerPairToStr(std::string const &first, std::string const &second);
std::string     stringExtract(std::string const &needle, std::string const &haystack);
std::string     trimAfter(std::string const &str, char const &c);
std::string     trimBefore(std::string const &str, char const &c);
std::ostream    &operator<<(std::ostream &os, std::map<std::string, std::string> const &m);
size_t 			ft_strlen(char const *s);
char			*ft_strdup(char const *s);
int				lseek_next_line(int fd, std::string &line);


#endif //SERV_UTILS_HPP
