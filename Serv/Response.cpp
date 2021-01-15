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
