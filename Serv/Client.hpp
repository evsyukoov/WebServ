//
// Created by Casie Carl on 1/5/21.
//

#ifndef SERV_CLIENT_HPP
#define SERV_CLIENT_HPP
#include <string>
#include "Parser/ServConf.hpp"
#include <netinet/in.h>
#include <map>

#define HEADER 0
#define BODY 1
#define BODY_CHUNKED 2
#define FINISH 3

class Client {

    int             client_sock;
    std::string     remoteAddr;
    std::string     request;
    ServConf        servConf;
    int             state;
    size_t          body_size;
    std::string     body;
    std::string     raw_body;
    std::string     piece;
    long            chunk_size;
    std::string 	chunk_end;

    std::map<std::string, std::string>  head;

    int     checkBodyHeaders(std::string splitted);

    void    analizeBodySize();

    int    decodeChunks();

    void    analizeChunked();

    bool    isDigit(const std::string &digit);

public:
    Client(int clientSock, const ServConf &servConf, sockaddr_in &sAddr);

    int getClientSock() const;

    void    clear();

    const ServConf &getServConf() const;



    std::string getRequest();
    std::string &getRemoteAddr();

    void                findState(std::string &piece);

    int getState() const;

    int    findBodySize(std::string header);

    int getBodySize() const;


};


#endif //SERV_CLIENT_HPP
