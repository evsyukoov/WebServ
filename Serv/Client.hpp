//
// Created by Casie Carl on 1/5/21.
//

#ifndef SERV_CLIENT_HPP
#define SERV_CLIENT_HPP
#include <string>
#include "Parser/ServConf.hpp"
#include <map>

#define HEADER 0
#define BODY 1
#define FINISH 2

class Client {

    int             client_sock;
    std::string     request;
    ServConf        servConf;
    int             state;
    int             body_size;
    std::string     body;
    int             current_body_size;
    std::map<std::string, std::string>  head;

    int     checkContentLength(std::string splitted);

    void    analizeBodySize();
public:
    Client(int clientSock, const ServConf &servConf);

    int getClientSock() const;

    void setPieceOfRequest(const std::string &pieceOfRequest);

    bool        addPieceOfRequest(const std::string &piece);

    void    clear();

    const ServConf &getServConf() const;



    std::string getRequest();

    void                findState(std::string &piece);

    int getState() const;

    int    findBodySize(std::string header);

    int getBodySize() const;


};


#endif //SERV_CLIENT_HPP
