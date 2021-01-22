#include "Response.hpp"
#include "../Utils/Debug.hpp"

Response::Response(int _sockFd, std::string const &_headers) :
sockFd(_sockFd), headers(_headers)
{
    buffer = new char[ R_SIZE + 1 ];
    bodyLength = 0;
}

Response::Response(const Response &copy) : sockFd(copy.sockFd), headers(copy.headers), bodyLength(copy.bodyLength)
{
	buffer = copy.buffer;
}

Response &Response::operator=(const Response &copy)
{
	if (this != &copy)
	{
		sockFd     = copy.sockFd;
		headers    = copy.headers;
		bodyLength = copy.bodyLength;
	}
	return *this;
}

Response::~Response()
{
	if (!tmpFile.empty())
		unlink(tmpFile.c_str());
    delete buffer;
}

ssize_t Response::sendHeader()
{
	int rv;

	rv = ::send(sockFd, headers.c_str(), headers.size(), MSG_DONTWAIT);

	return rv;
}

ssize_t Response::sendPiece() { return (bodyLength); };

ssize_t Response::sendChunk() { return (bodyLength); }

void Response::setTmpFile(const std::string &tmp_file)
{
	tmpFile = tmp_file;
}

static std::string     ulToHex(size_t num)
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

StringResponse::StringResponse(int _sockFd, std::string const &_headers, std::string const &_body)
: Response(_sockFd, _headers), body(_body)
{
	bodyLength = body.size();
}

StringResponse::StringResponse(const StringResponse &copy) : Response(copy)
{
	this->body = copy.body;
}

StringResponse &StringResponse::operator=(const StringResponse &copy)
{
	Response::operator=(copy);
	this->body = copy.body;
	return (*this);
}

ssize_t   StringResponse::sendPiece()
{
	ssize_t rv;
	size_t nbyte = std::min((size_t)bodyLength, (size_t)R_SIZE);
	rv = ::send(sockFd, body.c_str(), nbyte, MSG_DONTWAIT);
	if (rv > 0)
	{
		body.erase(rv);
		bodyLength -= rv;
	}
	return (bodyLength);
}

ssize_t StringResponse::sendChunk()
{
	return (0);
}

StringResponse::~StringResponse() {}

FileResponse::FileResponse(int _sockFd, std::string const &_headers, ssize_t const _bodyFd, ssize_t const _bodyLength)
: Response(_sockFd, _headers), bodyFd(_bodyFd)
{
	bodyLength = _bodyLength;
}

FileResponse::~FileResponse()
{
	close(bodyFd);
};

ssize_t FileResponse::sendPiece()
{
	ssize_t rv;

	size_t  nbytes;
	rv     = read(bodyFd, buffer, R_SIZE);

	if (rv > 0)
	{
		nbytes = std::min(rv, (ssize_t)R_SIZE);
		rv = ::send(sockFd, buffer, nbytes, MSG_DONTWAIT);
		rv = std::min(rv, (ssize_t)bodyLength);
		bodyLength -= (rv > 0) ? rv : 0;
	}
	return (bodyLength);
}

ssize_t FileResponse::sendChunk()
{
	size_t      nbytes = std::min(bodyLength, (size_t)R_SIZE);
	ssize_t     rv = read(bodyFd, buffer, nbytes);

	if (rv > 0)
	{
		nbytes = std::min(rv, (ssize_t)R_SIZE);
		std::string chunk = ulToHex(nbytes) + "\r\n" + buffer + "\r\n";
		bodyLength -= ::send(sockFd, chunk.c_str(), chunk.size(), MSG_DONTWAIT);
	}
	else if (bodyLength == 0)
		::send(sockFd, "0\r\n\r\n", 5, MSG_DONTWAIT);
	return (bodyLength);
}
