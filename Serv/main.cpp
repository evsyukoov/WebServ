#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("Client.hpp", O_RDONLY);
	std::string line;

	while (lseek_next_line(fd, line) > 0)
	{
		std::cout << line << std::endl;
	}
	read(0,0,1);
	return (0);
}
