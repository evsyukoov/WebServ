//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_SERVCONF_HPP
#define SERV_SERVCONF_HPP


#include <string>
#include <map>
#include "Location.hpp"
#include <iostream>
#include "parser_utils.hpp"
//конфиг одного сервера

class  ServConf {
	std::string					server_name;
	int 						port;
	std::map<int, std::string>	error_pages;
	std::list<Location>			locations;

	std::string                 raw_servconf;

	std::list<std::string>      raw_directives;

	int                         pos;

    int error(std::string msg);

    int     isErrorDelimetr(const std::string &directive);

    int     isLocationBlock(const std::string &directive);




public:
    ServConf(const std::string &rawServconf, int pos);

    int    parseRaw();

    int    parse();

    int   analizeDirective(std::list<std::string> line);

    const std::string &getServerName() const;

    int getPort() const;

    const std::map<int, std::string> &getErrorPages() const;

};


#endif //SERV_SERVCONF_HPP
