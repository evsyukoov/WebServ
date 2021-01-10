
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	struct timeval tv;
	struct tm time;
	std::string t_str;
	char buf[29];
	int fd;
	struct stat st;

	gettimeofday(&tv, NULL);

	fd = open("/Users/zcolleen/Desktop/webserv2/Serv/YoupiBanane/youpi.bad_extension", O_RDWR, 0644);

	fstat(fd, &st);
	std::cout << st.st_mtimespec.tv_sec << std::endl;
	t_str = std::to_string(st.st_mtimespec.tv_sec);

	strptime(const_cast<char*>(t_str.c_str()), "%s", &time);

	strftime(buf, 29, "%a, %d %b %Y %T GMT", &time);
	std::cout << buf << std::endl;
	std::cout << t_str << std::endl;
}

