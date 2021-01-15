#include "Response.hpp"

Response::Response(s_Response const &s) :
headerMap(s.headerMap), respLine(s.respLine), bodyFilename(s.bodyFilename)
{
    bodyLength = s.bodyLength;
    buffer = new char[ MBx1 + 1 ];
}

Response::~Response()
{
    delete buffer;
}

ssize_t   Response::sendPiece()
{
    size_t      nbytes = std::min(bodyLength, (ssize_t)MBx1);
    ssize_t     rv = read(bodyFd, buffer, nbytes);

    if (rv > 0)
    {
        nbytes = std::min(rv, (ssize_t)MBx1);
        bodyLength -= send(sockFd, buffer, nbytes, MSG_DONTWAIT);
    }
    return (bodyLength);
}

std::string     ulToHex(size_t num)
{
    static const char   *base16 = "0123456789abcdef";
    std::string         rv;

    while (num)
    {
        rv.insert(rv.begin(), base16[num % 16]);
        num /= 16;
    }
    return rv;
}

ssize_t   Response::sendChunk()
{
    size_t      nbytes = std::min(bodyLength, (ssize_t)MBx1);
    ssize_t     rv = read(bodyFd, buffer, nbytes);

    if (rv > 0)
    {
        nbytes = std::min(rv, (ssize_t)MBx1);
        std::string chunk = ulToHex(nbytes) + "\r\n" + buffer + "\r\n";
        bodyLength -= send(sockFd, buffer, nbytes, MSG_DONTWAIT);
    }
    else if (bodyLength == 0)
        send(sockFd, "0\r\n\r\n", 5, MSG_DONTWAIT);
    return (bodyLength);
}

int x_write(std::map<std::string, std::string> responseMap)
{
	int			localfd = open(responseMap["#file"].c_str(), O_RDONLY);
	ssize_t		nbyte;
	char		*buffer = new char[BUFFER_SIZE + 1];
	ssize_t		length = findFileSize(localfd) - std::stoul(respMap["#lseek"]);

	std::string respLine = "HTTP/1.1 " + respMap["Status"] + "\r\n";
	respLine += "Content-Length: " + std::to_string(length) + "\r\n";
	PRINT(respLine);
	lseek(localfd, std::stoul(respMap["#lseek"]), SEEK_SET); // start of body
	std::map<std::string, std::string>::iterator iter = this->respMap.begin();
	while (iter != respMap.end())
	{
		if (iter->first[0] != '#')
		{
			std::string field = headerPairToStr(*iter);
			respLine += field;
		}
		++iter;
	}
	respLine += "\r\n";
	write(client_fd, respLine.c_str(), respLine.size());
	while ((nbyte = read(localfd, buffer, BUFFER_SIZE)) > 0)
	{
		nbyte = std::min(length, nbyte);
		length -= write(client_fd, buffer, nbyte);
		if (length == 0)
			break ;
	}
	close(localfd);
	delete[] buffer;
	unlink(respMap["#file"].c_str());
	return (0);
}