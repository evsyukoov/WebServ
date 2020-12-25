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