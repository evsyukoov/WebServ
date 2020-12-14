//
// Created by Денис on 14/12/2020.
//

#include <iostream>
#include "HttpRequest.hpp"


HttpRequest::HttpRequest(char *buff, int len) {
	this->buff = buff;
	this->len = len;
	initMap();
}

const std::map<std::string, std::string> &HttpRequest::getRequestMap() const {
	return requestMap;
}

void HttpRequest::initMap() {
	int i = 0;
	while (buff[i] != '\n')
	{
		std::string val;
		while (buff[i] != ' ')
			val += buff[i++];
		i++;
		requestMap["method"] = val;
		val = "";
		while (buff[i] != ' ')
			val += buff[i++];
		i++;
		requestMap["location"] = val;
		val = "";
		while (buff[i] != '\n')
			val += buff[i++];
		i++;
		requestMap["protocol"] = val;
		break ;
	}
}

void 	HttpRequest::printMap()
{
	std::map<std::string, std::string>::iterator i1 = requestMap.begin();
	std::map<std::string, std::string>::iterator i2 = requestMap.end();
	while (i1 != i2)
	{
		std::cout << i1->first << ": " << i1->second << std::endl;
		i1++;
	}
}

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {

}





