//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_LOCATION_HPP
#define SERV_LOCATION_HPP
#include <string>
#include <vector>
#include "parser_utils.hpp"
#include "../Utils/utils.hpp"
//конфиг одного локейшна


class Location {
	//after parse
	std::string 		   location;

	std::vector<std::string> methods;

	int                     max_body;

	std::string			   root;

	std::string			   index;

	std::string			   cgi_extension;

	std::string			   cgi_scrypt;

	std::string            interpretator;

	std::string             cgi_index;

	bool 				   autoindex;

	//input
	std::string            raw_location;

	std::list<std::string>  raw_directives;

    int     parse();

    int     analizeMethods();

    int     analizeDirective(std::list<std::string> &directives);

    int     isRightDirective(const std::string &direct, const std::string &method);

    int    parseMethods(std::string &directive, int ind);

    int     isAllowedMethod(std::string &method);

    int     ifFileExists(const std::string &fileName);

public:
    Location(const std::string &rawLocation);

    int     parseRaw();

    int parseLocation(const std::string &loc);

    const std::string &getLocation() const;

    const std::vector<std::string> &getMethods() const;

    int getMaxBody() const;

    const std::string &getRoot() const;

    const std::string &getIndex() const;

    const std::string &getCgiIndex() const;

    const std::string &getCgiExtension() const;

    const std::string &getCgiScrypt() const;

    bool isAutoindex() const;

    Location	&operator=(const Location &other);

    Location(const Location &other);

	Location();

	virtual ~Location();

    const std::string &getInterpretator() const;


};


#endif //SERV_LOCATION_HPP
