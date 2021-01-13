//
// Created by Casie Carl on 12/15/20.
//



#include <fstream>
#include "CGI.hpp"

CGI::CGI(const t_cgi &struct_cgi, const ServConf &servConf, input &in) : in(in) {
	this->struct_cgi = struct_cgi;
	this->servConf = servConf;
	initEnvironments();
	initARGS();
}

int        CGI::initARGS()
{
	if (in.interptretator.empty())
	{
		if (!(args = (char**)malloc(sizeof (char*) * 2)))
			return (error("Malloc Error init args to execve"));
		args[0] = (char*)in.scrypt.c_str();
		args[1] = NULL;
	}
	else
	{
		if (!(args = (char**)malloc(sizeof (char*) * 3)))
			return (error("Malloc Error init args to execve"));
		args = (char**)malloc(sizeof(char*) * 3);
		args[0] = (char*)in.interptretator.c_str();
		args[1] = (char*)in.scrypt.c_str();
		args[2] = 0;
	}
	return (1);
}

void        CGI::initEnvironments()
{
	environments["AUTH_TYPE"] = "Basic";
	environments["CONTENT_LENGTH"] = std::to_string(struct_cgi.content_length);//std::to_string(request.size());
	environments["CONTENT_TYPE"] = struct_cgi.content_type;
	environments["GATEWAY_INTERFACE"] = "CGI/1.1";
	environments["PATH_INFO"] = "/" ;        //путь к скрипту(относительный)
	environments["PATH_TRANSLATED"] = "/Users/zcolleen/Desktop/webserv2/Serv/example.cgi";
	//@TODO environments["QUERY_STRING"] = struct_cgi.query_string;
	//environments["REMOTE_ADDR"] = servConf.getServerName();
	//environments["REMOTE_IDENT"]  =
	//environments["REMOTE_USER"] = ;
	environments["REQUEST_METHOD"] = struct_cgi.reques_method;
	//  environments["REQUEST_URI"] = "/directory/youpi.bla";
	environments["SERVER_NAME"] = servConf.getServerName();
	environments["SERVER_PORT"] = std::to_string(servConf.getPort());
	environments["SERVER_PROTOCOL"] = "HTTP/1.1";
	environments["SERVER_SOFTWARE"] = "webserv";
	environments["HTTP_X-Secret-Header-For-Test"] = "1";
}

int     CGI::mapToEnv()
{
	if (!(env = (char**)malloc(sizeof(char*) * (environments.size() + 1))))
		return (0);
	int i = 0;
	for(std::map<std::string, std::string>::iterator it = environments.begin(); it != environments.end(); it++)
	{
		std::string envir = (it->first + "=" + it->second);
		env[i] = (char*)malloc(sizeof(char) * (envir.size() + 1));
		int j = 0;
		while (j < envir.size()) {
			env[i][j] = envir[j];
			j++;
		}
		env[i][j] = '\0';
		i++;
	}
	env[i] = 0;
	return (1);
}

int	CGI::run()
{
	int fd[2];
	int status;
	mapToEnv();
	if (pipe(fd) < 0)
		throw (std::runtime_error(strerror(errno)));
	int child = fork();
	int tmp_fd = open("./tmp", O_RDWR | O_CREAT, S_IRWXU);
	if (tmp_fd < 0)
		throw (std::runtime_error(strerror(errno)));
	if (child < 0) {
		throw (std::runtime_error(strerror(errno)));
	}
	else if (child == 0)
	{
		close(fd[1]); //ничего не пишем
		//заменяем stdout дочернего процесса на дескриптор временного файла
		dup2(tmp_fd, 1);
		close(tmp_fd);
		//читать запрос будем от родителя
		dup2(fd[0], 0);
		close(fd[0]);
		if (execve(args[0],  args, env) == -1)
		{
			std::cerr << "Problems with execve" << std::endl;
			exit(EXIT_FAILURE);
		}
		//std::cout << "CHILD" << std::endl;
		exit (EXIT_SUCCESS);
	}
		//родитель
	else
	{
		close(fd[0]);
		//dup2(fd[1],1);
		write(fd[1], struct_cgi.query_string.c_str(), struct_cgi.query_string.size());
		close(fd[1]);
		waitpid(child, &status, 0);
		readFromCGI();
	}
	return (1);
}

int     CGI::readFromCGI()
{
	int fd = open("./tmp", O_RDONLY);
	if (fd < 0)
		throw (std::runtime_error(strerror(errno)));

	long size = findFileSize(fd);
	if (size < 0)
		throw (std::runtime_error(strerror(errno)));

	std::cout << "HELLO from cgi" << std::endl;
	char *buff;
	if (!(buff = (char*)malloc(sizeof(char) * (size + 1))))
		throw (std::runtime_error(strerror(errno)));
	int n = read(fd, buff, size);
	buff[n] = '\0';
	response = buff;
	unlink("./tmp");
//    map["status"] = "";
//    map["content-type"] = "";
//    map["body"] = "";
	return (1);
}

const std::string &CGI::getResponse() const {
	return response;
}







