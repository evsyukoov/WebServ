//
// Created by Casie Carl on 1/5/21.
//

#include "Client.hpp"
#include "Server.hpp"

Client::Client(int clientSock, const ServConf &servConf)
{
    this->client_sock = clientSock;
    this->servConf = servConf;
    this->request = "";
}

int Client::getClientSock() const {
    return client_sock;
}

void Client::setPieceOfRequest(const std::string &request) {
    this->request = request;
}

bool Client::addPieceOfRequest(std::string piece)
{
    //если не встретили \r\n\r\n то продолжаем наращивать запрос
    request += piece;
    std::cout << BLUE << "piece: " << piece << RESET << std::endl;
    if (request.substr(request.size() - 4) == "\r\n\r\n") {
        std::cout << "true" << std::endl;
        return (true);
    }
    else
        return (false);
}

void Client::clearRequest() {
    request.clear();
}

const ServConf &Client::getServConf() const {
    return servConf;
}

const std::string &Client::getRequest() const {
    return request;
}

