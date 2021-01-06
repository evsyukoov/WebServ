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
    this->body = "";
    state = HEADER;
}

int Client::getClientSock() const {
    return client_sock;
}

void Client::setPieceOfRequest(const std::string &request) {
    this->request = request;
}

bool Client::addPieceOfRequest(const std::string &piece)
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


const ServConf &Client::getServConf() const {
    return servConf;
}

std::string Client::getRequest(){
    std::string res = request + body;
    return res;
}

int     Client::checkContentLength(std::string splitted)
{
    int pos_dot = splitted.find(':');
    if (pos_dot != std::string::npos) {
        std::string key = splitted.substr(0, pos_dot);
        if (key == "Content-Length") {
            body_size = std::atoi(splitted.substr(pos_dot + 2).c_str());
            return (1);
        }
    }
    return (0);
}

int    Client::findBodySize(std::string header)
{
    int begin = 0;
    int pos;
    std::string splitted;

    while ((pos = header.find("\r\n")) != std::string::npos)
    {
        splitted = header.substr(0, pos);
        begin = pos + 2;
        if (checkContentLength(splitted))
            return (1);
        header = header.substr(begin);
    }
    return (0);
}

void    Client::analizeBodySize()
{
    if (body.size() < body_size)
        state = BODY;
    else if (body.size() == body_size)
        state = FINISH;
    else {
        body = body.substr(0, body_size);
        state = FINISH;
    }
}

void Client::findState(std::string &piece)
{
    int     delimetr;

    if (state == HEADER) {
        request += piece;
        if ((delimetr = request.find("\r\n\r\n")) != std::string::npos) {
            std::string header = request.substr(0, delimetr + 2);
            if (!findBodySize(header))
                state = FINISH;
            else
            {
                body = request.substr(delimetr + 4);
                request = request.substr(0, delimetr + 4);
                analizeBodySize();
            }
        }
    }
    else if (state == BODY)
    {
        body += piece;
        analizeBodySize();
    }
}

int Client::getState() const {
    return state;
}

void Client::clear() {
    state = HEADER;
    body.clear();
    request.clear();
}

int Client::getBodySize() const {
    return body_size;
}
