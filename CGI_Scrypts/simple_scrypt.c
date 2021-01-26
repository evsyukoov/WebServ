//
// Created by Casie Carl on 12/15/20.
//

#include <unistd.h>
#include <stdio.h>

int 	main(int argc, char **argv, char **env)
{
    int i = 1;
    char buff[100];
    int n = read(0, buff, 100);
    buff[n] = '\0';
    pritf("%s modified by child\n", buff);
//	while(env && env[i])
//	    std::cout << env[i++] << std::endl;
}

