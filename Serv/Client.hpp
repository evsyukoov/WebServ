//
// Created by Casie Carl on 1/5/21.
//

#ifndef SERV_CLIENT_HPP
#define SERV_CLIENT_HPP
#include <string>
#include "Parser/ServConf.hpp"
#include <map>
#include <netinet/in.h>

#define HEADER 0
#define BODY 1
#define BODY_CHUNKED 2
#define FINISH 3

class Client {

    int             client_sock;
    sockaddr_in     sAddr;
    std::string     remoteAddr;
    std::string     request;
    ServConf        servConf;
    int             state;
    int             body_size;
    std::string     body;
    std::string     raw_body;
    std::string     piece;
    long             chunk_size;
    std::string 	chunk_end;

    std::map<std::string, std::string>  head;

    int     checkBodyHeaders(std::string splitted);

    void    analizeBodySize();

    int     decodeChunks();

    void    analizeChunked();

    bool    isDigit(const std::string &digit);

public:
    Client(int clientSock, const ServConf &servConf, sockaddr_in &sAddr);

    void    clear();

    void            findState(std::string &piece);

    const ServConf  &getServConf() const;
    std::string     getRequest();
    int             getClientSock() const;
    std::string     &getRemoteAddr();
    int             getState() const;

    int    findBodySize(std::string header);

    int getBodySize() const;


};


#endif //SERV_CLIENT_HPP
