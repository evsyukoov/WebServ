//
// Created by Casie Carl on 1/5/21.
//

#ifndef SERV_CLIENT_HPP
#define SERV_CLIENT_HPP
#include <string>
#include "../Parser/ServConf.hpp"
#include <netinet/in.h>
#include "Response.hpp"
#include <map>
#include "../Request_Processing/HTTP.hpp"

#define HEADER 0
#define BODY 1
#define BODY_CHUNKED 2
#define WRITING 4
#define WRITING_BODY 5
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

    std::map<std::string, std::string> reqMap;
public:
    const std::string &getBody() const;

    void setServConf(const ServConf &servConf);

public:
    const std::map<std::string, std::string> &getReqMap() const;


private:

    std::map<std::string, std::string>  head;
    Response *resp;

    int     checkBodyHeaders(std::string const &splitted);

    void    analizeBodySize();

    int     decodeChunks();

    void    analizeChunked();

public:
    Client(int clientSock, sockaddr_in &sAddr);

    int     getClientSock() const;

    void    clear();

    const ServConf &getServConf() const;



    std::string getRequest();
    std::string &getRemoteAddr();

    void    findState(std::string &_piece);

    int     getState() const;

    int     findBodySize(std::string header);

    int     getBodySize() const;

    Response    *getResponse();
	void        setResponse(Response *r);

	void        clearRequest();

};


#endif //SERV_CLIENT_HPP
