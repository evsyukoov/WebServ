//
// Created by Casie Carl on 12/15/20.
//

#ifndef SERV_CGI_HPP
#define SERV_CGI_HPP
#include <zconf.h>

// 	CGI_scrypt(дочерний процесс)	----->	Server --- > Client(Browser)
								//	<-----		 //<----

class CGI
{
	//имя CGI скрипта
	char *child_program;
	char **env;
	char **args;

	// запрос от сервера на CGI
	char *request;

public:
	CGI(char *childProgram, char **env, char **args, char *request);

	int 	run_child();

	void 	run_parent();
};


#endif //SERV_CGI_HPP
