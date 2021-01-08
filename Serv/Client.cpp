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

const ServConf &Client::getServConf() const {
    return servConf;
}

std::string Client::getRequest(){
    std::string res = request + body;
    return res;
}

int     Client::checkBodyHeaders(std::string splitted)
{
    int pos_dot = splitted.find(':');
    if (pos_dot != std::string::npos) {
        std::string key = splitted.substr(0, pos_dot);
        if (key == "Content-Length") {
            body_size = std::atoi(splitted.substr(pos_dot + 2).c_str());
            return (1);
        }
        else if (key == "Transfer-Encoding")
        {
            std::string val = splitted.substr(pos_dot + 2).c_str();
            if (val.find("chunked") != std::string::npos)
                return (2);
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
        if (checkBodyHeaders(splitted) == 1)
            return (1);
        else if (checkBodyHeaders(splitted) == 2)
            return (2);
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

bool    Client::isDigit(const std::string &digit)
{
    std::string available = "0123456789aAbBcCdDeEfF";
    for(int i = 0; i < digit.size(); i++)
    {
        if (available.find(digit[i]) == std::string::npos)
            return false;
    }
    return true;
}

int    Client::decodeChunks(std::string input)
{
    //input = 4 \r\n abcd \r\n 3\r\n qwe\r\n 0 \r\n\r\n -пробелы для читаемости
    int pos = 0;
    int line = 0;
    long hex;
    while ((pos = input.find("\r\n")) != std::string::npos)
    {
        if (line % 2 == 0) {
            std::string digit = input.substr(0, pos);
            if (!isDigit(digit))
                return (0);
            hex = std::strtol(input.substr(0, pos).c_str(), NULL, 16);
        }
        else
        {
            std::string content = input.substr(0, pos);
            body += content.substr(0, hex);
        }
        input = input.substr(pos + 2);
        line++;
    }
    return (1);
}

void    Client::analizeChunked()
{
    if (raw_body.find("0\r\n\r\n") != std::string::npos)
    {
        state = FINISH;
        raw_body = raw_body.substr(0, raw_body.size() - 5);
        if (!decodeChunks(raw_body))
            request = "UNKNOWN METHOD HTTP/1.1";
    }
}

void Client::findState(std::string &piece) {
    int delimetr = 0;

    if (state == HEADER) {
        request += piece;
        if ((delimetr = request.find("\r\n\r\n")) != std::string::npos) {
            std::string header = request.substr(0, delimetr + 2);
            int ret = findBodySize(header);
            if (!ret)
                state = FINISH;
            else if (ret == 1) {
                body = request.substr(delimetr + 4);
                request = request.substr(0, delimetr + 4);
                analizeBodySize();
            } else if (ret == 2) {
                state = BODY_CHUNKED;
                raw_body = request.substr(delimetr + 4);
                request = request.substr(0, delimetr + 4);
                analizeChunked();
            }

        }
    } else if (state == BODY) {
        body += piece;
        analizeBodySize();
    }
    else if (state == BODY_CHUNKED)
    {
        raw_body += piece;
        analizeChunked();
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
