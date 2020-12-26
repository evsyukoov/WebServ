//
// Created by Casie Carl on 12/15/20.
//

#include <iostream>
#include <unistd.h>

int 	main(int argc, char **argv, char **env)
{
    int i = 1;
    char buff[100];
    int n = read(0, buff, 100);
    buff[n] = '\0';
    std::cout << buff << " modified by child " << std::endl;
//	while(argv && argv[i])
//	    std::cout << argv[i++] << std::endl;
}

