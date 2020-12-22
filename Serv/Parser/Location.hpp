//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_LOCATION_HPP
#define SERV_LOCATION_HPP
#include <string>
#include <list>
#include <vector>
#include "parser_utils.hpp"
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
	bool 				   autoindex;
	bool 				   enable_upload;
	std::string			   upload_path;

	//input
	std::string            raw_location;

	std::list<std::string>  raw_directives;

    int     parse();

    int     analizeMethods();

    int     analizeDirective(std::list<std::string> &directives);

    int     isRightDirective(const std::string &direct, const std::string &method);

    int    parseMethods(std::string &directive, int ind);

    int     isAllowedMethod(const std::string &method);

    int     findEndOfKeyword(const std::string &word);

public:
    Location(const std::string &rawLocation);

    int     parseRaw();

    //int     parseLocation();

    int parseLocation(const std::string &loc);

    const std::string &getLocation() const;

    const std::vector<std::string> &getMethods() const;

    int getMaxBody() const;

    const std::string &getRoot() const;

    const std::string &getIndex() const;

    const std::string &getCgiExtension() const;

    const std::string &getCgiScrypt() const;

    bool isAutoindex() const;

    bool isEnableUpload() const;

    const std::string &getUploadPath() const;

    Location	&operator=(const Location &other);

    Location(const Location &other);

	Location();

	virtual ~Location();


};


#endif //SERV_LOCATION_HPP
