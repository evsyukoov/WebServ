//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_SERVCONF_HPP
#define SERV_SERVCONF_HPP


#include <string>
#include <map>
#include "Location.hpp"

//конфиг одного сервера

class  ServConf {
	std::string					path_to_conf;
	std::string					server_name;
	int 						port;
	std::map<int, std::string>	error_pages;
	std::list<Location>			locations;

};


#endif //SERV_SERVCONF_HPP
