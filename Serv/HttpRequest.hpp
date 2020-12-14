//
// Created by Денис on 14/12/2020.
//

#ifndef SERV_HTTPREQUEST_HPP
#define SERV_HTTPREQUEST_HPP
#define MAX_LEN = 4096
#include <map>
#include <string>
#include "lib.hpp"

//то что пришло от пользователя серверу

class HttpRequest {
	char *buff;
	int len;
	std::map<std::string, std::string> requestMap;
	void 	skip(char c);

public:

	HttpRequest();

	HttpRequest(char *buff, int len);

	const std::map<std::string, std::string> &getRequestMap() const;

	void 	initMap();

	virtual ~HttpRequest();

	void 	printMap();  //для отладки

};


#endif //SERV_HTTPREQUEST_HPP
