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
#include "Parser/Config.hpp"
#include "HTTP.hpp"

// 	CGI_scrypt(дочерний процесс)	----->	Server --- > Client(Browser)
								//	<-----		 //<----

class CGI
{


//имя CGI скрипта
	char **env;
	char **args;
	input &in;

	//настройки конфига
	ServConf servConf;

	// запрос от сервера на CGI
	t_cgi cgi;

	//ответ после обработки

	std::string response;

	std::map<std::string, std::string> environments;

    int    readFromCGI();

    void       initEnvironments();

    int        initARGS();

    int     mapToEnv();

public:
    CGI(const t_cgi &cgi, const ServConf &servConf, input &in);

	int 	run();



    const std::string &getResponse() const;

};


#endif //SERV_CGI_HPP
