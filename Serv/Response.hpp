
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include <fcntl.h>
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>

class Response
{
 #define MBx1 1000000
	int                 sockFd;

	std::string const   headers;
	std::string const   bodyFilename;

	int                 bodyFd;
	ssize_t             bodyLength;

	char    *buffer;

	int lseek_next_line(int fd, std::string &line);


 public:
	Response(int _sockFd, std::string const &_headers, ssize_t const _bodyFd, ssize_t const _bodyLength);
	Response(int _sockFd, std::string const &_headers, std::string const &_body);
	~Response();
	void    setHeaderMap(std::map<std::string, std::string> const map);
	void    setRespLine(std::string const &respLine);
	int     send();
	ssize_t sendPiece();
	ssize_t sendChunk();
};

#endif
