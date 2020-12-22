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
#include <vector>
//конфиг одного сервера

class  ServConf {
    //output data
	std::string					server_name;

	int 						port;

	std::string 				root;

	std::string 				index;

	std::map<int, std::string>	error_pages;

	std::list<Location>			locations;

	//input and helper data
	std::string                 raw_servconf;

	std::list<std::string>      raw_directives;

	int                         pos;

    int     isLocationBlock(const std::string &directive);

    int   analizeDirective(std::list<std::string> line);


public:
    ServConf(const std::string &rawServconf, int pos);

    int    parseRaw();

    int    parse();

    const std::string &getServerName() const;

    int getPort() const;

    const std::list<Location> &getLocations() const;

    const std::map<int, std::string> &getErrorPages() const;

	const std::string &getRoot() const;

	const std::string &getIndex() const;

	ServConf	&operator=(const ServConf &other);

	ServConf(const ServConf &other);

	ServConf();

	virtual ~ServConf();

};


#endif //SERV_SERVCONF_HPP
