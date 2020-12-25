//
// Created by Casie Carl on 12/15/20.
//

#ifndef SERV_CGI_HPP
#define SERV_CGI_HPP
#include <zconf.h>
#include <iostream>
#include <fcntl.h>
#include <map>
#include "utils.hpp"

// 	CGI_scrypt(дочерний процесс)	----->	Server --- > Client(Browser)
								//	<-----		 //<----

class CGI
{
	//имя CGI скрипта
	char *child_program;
	char **env;
	char **args;

	// запрос от сервера на CGI
	std::string request;

	//ответ после обработки

	std::string response;

	std::map<std::string, std::string> environments;

    int    readFromCGI();

    void       initEnvironments();

public:
	CGI(char *childProgram, char **env, char **args, const std::string &request);

	int 	run();



    const std::string &getResponse() const;
};


#endif //SERV_CGI_HPP
