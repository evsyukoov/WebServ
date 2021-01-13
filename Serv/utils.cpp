//
// Created by Casie Carl on 12/25/20.
//

#include "utils.hpp"

long  findFileSize(int fd)
{
    struct stat file_stat;
    int res = fstat(fd, &file_stat);
    if (res < 0)
        return (-1);
    return file_stat.st_size;
}

size_t  ft_strlen(char const *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

char   *ft_strdup(char const *str)
{
    int i = 0;
    char *res;

    if (!(res = (char*)malloc(sizeof(char) * (ft_strlen(str) + 1))))
        return NULL;
    while (str[i])
    {
        res[i] = str[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}

void inet_toip4(void *sAddr, std::string &buffer)
{
    unsigned char *sss = reinterpret_cast<unsigned char *>(sAddr);

    buffer.clear();
    for (int i = 0; i < 4; ++i)
        buffer += std::to_string(int(sss[i])) + '.';
    buffer.pop_back();
}

void    stringExtract(std::string const &needle, std::string &haystack)
{
    size_t delim = haystack.find(needle);
    std::string tmp;

    if (delim != std::string::npos)
    {
        tmp = haystack.substr(0, delim);
        haystack = haystack.substr(delim + needle.size(), haystack.size());
    }
}

std::ostream    &operator<<(std::ostream &os, std::map<std::string, std::string> const &m)
{
    std::map<std::string, std::string>::const_iterator it = m.begin();
    while (it != m.end())
    {
        std::cout << it->first << ": " << it->second << std::endl;
        ++it;
    }
}