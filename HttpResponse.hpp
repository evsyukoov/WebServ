//
// Created by Денис on 14/12/2020.
//

#ifndef SERV_HTTPRESPONSE_HPP
#define SERV_HTTPRESPONSE_HPP
#include "HttpRequest.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <zconf.h>
#include "Net.hpp"
#define OK "HTTP/1.1 200 OK\nContent-type: text/html\n\n"

//тут будет ответ от сервера клиенту в соответствии с запросом (запрос в reqMap)


class HttpResponse {
	std::map<std::string, std::string> reqMap;  //наша map из Request'a
	int				connection;
	std::string		fileName; //index.html
	std::string 	root; //root-откуда начинать искать
	Net				net;


public:
	HttpResponse(const Net &net, const std::map<std::string, std::string> &reqMap, int connection, const std::string &fileName, const std::string &root);

	void 	get();

	virtual ~HttpResponse();

};


#endif //SERV_HTTPRESPONSE_HPP
