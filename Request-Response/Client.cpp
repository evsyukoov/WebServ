//
// Created by Casie Carl on 1/5/21.
//

#include "Client.hpp"
#include "../Server/Server.hpp"

Client::Client(int clientSock, sockaddr_in &sAddr)
{
    inet_toip4(&sAddr.sin_addr, this->remoteAddr);
    this->client_sock = clientSock;
    this->request = "";
    this->body = "";
    body_size = 0;
    chunk_size = 0;
    state = HEADER;
    resp = NULL;
	//std::cout << "Client " << this->remoteAddr;
	//std::cout << " connected to server." << std::endl;
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

std::string &Client::getRemoteAddr() {
    return remoteAddr;
}



int     Client::checkBodyHeaders(std::string const &splitted)
{
    size_t pos_dot = splitted.find(':');
    if (pos_dot != std::string::npos) {
        std::string key = splitted.substr(0, pos_dot);
        if (key == "Content-Length") {
            body_size = std::atoi(splitted.substr(pos_dot + 2).c_str());
            return (1);
        }
        else if (key == "Transfer-Encoding")
        {
            std::string val = splitted.substr(pos_dot + 2);
            if (val.find("chunked") != std::string::npos)
                return (2);
        }
    }
    return (0);
}

int    Client::findBodySize(std::string header)
{
    size_t begin;
    size_t pos;
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
        state = WRITING;
    else {
        body = body.substr(0, body_size);
        state = WRITING;
    }
}

static bool isHexDigit(const std::string &digit)
{
    std::string available = "0123456789aAbBcCdDeEfF";
    for(size_t i = 0; i < digit.size(); i++)
    {
        if (available.find(digit[i]) == std::string::npos)
            return false;
    }
    return true;
}

int    Client::decodeChunks()
{
    //input = 10 \r\n abcdefgrty \r\n 3\r\n qwe\r\n 0 \r\n\r\n -пробелы для читаемости
    size_t pos;
    if ((raw_body.find("0\r\n\r\n") != std::string::npos) || chunk_end == "0\r\n\r\n") {
        state = WRITING;
    }
    while ((pos = raw_body.find("\r\n")) != std::string::npos)
    {
        if (!chunk_size)
        {
            std::string digit = raw_body.substr(0, pos);
			if (digit.size() == 1 && digit[0] == '0')
				chunk_end = raw_body.substr(pos - 1);
            if (!isHexDigit(digit))
                return (0);
            chunk_size = std::strtol(digit.c_str(), nullptr, 16);
        }
        else
        {
        	if (state == WRITING && chunk_size == 0)
        		return (1);
            std::string content = raw_body.substr(0, pos);
            piece = content.substr(0, chunk_size);
            body += piece;
            chunk_size = 0;
        }
        raw_body = raw_body.substr(pos + 2);
    }
    return (1);
}


void Client::findState(std::string &_piece) {
    size_t delimetr;

    if (state == HEADER) {
        request += _piece;
        if ((delimetr = request.find("\r\n\r\n")) != std::string::npos) {
            std::string header = request.substr(0, delimetr + 4);
            reqMap = HTTP::parseMap(header);
            if (reqMap.empty())
                state = WRITING;
            if ((reqMap.count("Content-Length") == 0 && reqMap.count("Transfer-Encoding") == 1 && reqMap["Transfer-Encoding"] != "chunked")
            || (reqMap.count("Transfer-Encoding") == 0 && reqMap.count("Content-Length") == 0))
                state = WRITING;
            else if (reqMap.count("Content-Length") != 0) {
                body_size = (size_t)std::stoi(reqMap["Content-Length"]);
                body = request.substr(delimetr + 4);
                request = request.substr(0, delimetr + 4);
                analizeBodySize();
            } else if (reqMap["Transfer-Encoding"] == "chunked") {
                state = BODY_CHUNKED;
                raw_body = request.substr(delimetr + 4);
                request = request.substr(0, delimetr + 4);
                decodeChunks();
            }

        }
    } else if (state == BODY) {
        body += _piece;
        std::cout << reqMap << std::endl;
        analizeBodySize();
    }
    else if (state == BODY_CHUNKED)
    {
        raw_body += _piece;
        if (!chunk_end.empty()) {
			chunk_end += _piece;
		}
        decodeChunks();
    }
}

void Client::setResponse(Response * r)
{
	resp = r;
	state = WRITING_BODY;
};

Response    *Client::getResponse()
{
	return resp;
}

int Client::getState() const {
    return state;
}

void Client::clear() {
    state = HEADER;
    body.clear();
    request.clear();
    raw_body.clear();
    chunk_size = 0;
    chunk_end.clear();
    delete this->resp;
    resp = NULL;
}

int Client::getBodySize() const {
    return body_size;
}

const std::map<std::string, std::string> &Client::getReqMap() const {
    return reqMap;
}

const std::string &Client::getBody() const {
    return body;
}

void Client::setServConf(const ServConf &servConf) {
    this->servConf = servConf;
}

void Client::clearRequest() {
    reqMap.clear();
}
