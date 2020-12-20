//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_LOCATION_HPP
#define SERV_LOCATION_HPP
#include <string>
#include <list>
#include "parser_utils.hpp"
//конфиг одного локейшна

class Location {
	std::string 		   location;
	std::list<std::string> methods;
	std::string			   root;
	std::string			   index;
	std::string			   cgi_extension;
	std::string			   cgi_scrypt;
	int 				   autoindex;
	int 				   enable_upload;
	std::string			   upload_path;

	std::string            raw_location;
public:
    Location(const std::string &rawLocation);

    int     parseRaw();

    //int     parseLocation();

    int parseLocation(const std::string &loc);

    const std::string &getLocation() const;

};


#endif //SERV_LOCATION_HPP
