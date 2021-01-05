//
// Created by Casie Carl on 1/5/21.
//

#ifndef SERV_CLIENT_HPP
#define SERV_CLIENT_HPP
#include <string>
#include "Parser/ServConf.hpp"

class Client {

    int             client_sock;
    std::string     request;
    ServConf        servConf;

public:
    Client(int clientSock, const ServConf &servConf);

    int getClientSock() const;

    void setPieceOfRequest(const std::string &pieceOfRequest);

    bool        addPieceOfRequest(std::string piece);

    void        clearRequest();

    const ServConf &getServConf() const;

    const std::string &getRequest() const;


};


#endif //SERV_CLIENT_HPP
