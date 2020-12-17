//
// Created by Casie Carl on 12/15/20.
//

#include <iostream>
#include <unistd.h>

int 	main(int argc, char **argv, char **env)
{
//	int i = 0;
//	while (i < argc)
//	{
//		std::cout << argv[i] << std::endl;
//		i++;
//	}
	std::cout << "Child process started" << std::endl;
	char buff[10];
	int nread = read(0, buff, 10);
	std::cout << "Read in child from parent" << std::endl;
	for (int i = 0; i < nread; i++)
	{
		std::cout << buff[i] << std::endl;
	}
}

