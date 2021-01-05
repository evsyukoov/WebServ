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

size_t  ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

char   *ft_strdup(char *str)
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