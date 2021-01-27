
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include <fcntl.h>
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <algorithm>

class Response
{
 protected:
 #define MBx1 1000000
 #define MBx10 10000000
 #define MBx100 100000000
 #define R_SIZE MBx10

	int                 sockFd;
	std::string         headers;
	size_t              bodyLength;
	std::string         tmpFile;
	bool                is_close;

	char    *buffer;

 public:
	Response() {};
	Response(int _sockFd, std::string const &_headers);
	Response(const Response &copy);
	Response &operator=(const Response &copy);
	void setTmpFile(std::string const &);
	virtual ~Response();
	//void    setHeaderMap(std::map<std::string, std::string> const map);
	//void    setRespLine(std::string const &respLine);
	ssize_t sendHeader();
	virtual ssize_t sendPiece();
	virtual ssize_t sendChunk();

    bool isClose() const;

    void setIsClose(bool isClose);
};

class StringResponse : public Response
{
	std::string body;

 public:
	StringResponse(int _sockFd, std::string const &_headers, std::string const &_body);
	StringResponse(StringResponse const &copy);
	StringResponse &operator=(StringResponse const &copy);
	~StringResponse();

	ssize_t sendPiece();
	ssize_t sendChunk();
};

class FileResponse : public Response
{
	int     bodyFd;

 public:
	FileResponse(int _sockFd, std::string const &_headers,
			  ssize_t const _bodyFd, ssize_t const _bodyLength);
	~FileResponse();
	ssize_t sendPiece();
	ssize_t sendChunk();
};

#endif
