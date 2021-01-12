//
// Created by Casie Carl on 12/15/20.
//

#include <fstream>
#include "CGI.hpp"

CGI::CGI(const s_cgi &struct_cgi, const ServConf &servConf, input &in) : in(in), _cgi(struct_cgi), reqMap(struct_cgi.reqMap) {
    this->servConf = servConf;
    this->env = NULL;
    this->buff = NULL;
	this->body = _cgi.reqMap->operator[]("body");
    initEnvironments();
    mapToEnv();
    initARGS();
}

int        CGI::initARGS()
{
    if (in.interptretator.empty())
    {
        args[0] = (char*)in.scrypt.c_str();
        args[1] = NULL;
    }
    else
    {
        args[0] = (char*)in.interptretator.c_str();
        args[1] = (char*)in.scrypt.c_str();
        args[2] = 0;
    }
    return (1);
}

static void	trimQString(std::string &str)
{
	size_t delim;
	if ((delim != str.find('?')) != str.npos)
		str.erase(0, delim);
}

void        CGI::initEnvironments()
{
    std::string host;

    environments["SERVER_NAME"] = servConf.getServerName();
    environments["SERVER_PORT"] = std::to_string(servConf.getPort());

   	host = (*reqMap).find("Host")->second;
    environments["PATH_INFO"] = "/"; //difficult without cgi extension (.bla, .cgi, .php...)
    environments["PATH_TRANSLATED"] = host + environments["PATH_INFO"];

    environments["CONTENT_LENGTH"] = std::to_string(_cgi.content_length);
    environments["CONTENT_TYPE"] = _cgi.content_type;
    environments["GATEWAY_INTERFACE"] = "CGI/1.1";

    environments["REQUEST_METHOD"] = (*reqMap).find("method")->second;
    environments["REQUEST_URI"] = (*reqMap).find("location")->second;
    environments["QUERY_STRING"] = environments["REQUEST_URI"];
	trimQString(environments["QUERY_STRING"]);

	if ((*_cgi.reqMap).count("Authorization"))
	{
		environments["AUTH_TYPE"] = "Basic"; // auth header
		environments["REMOTE_ADDR"] = _cgi.remoteAddr;
		//environments["REMOTE_IDENT"]  = // same
		//environments["REMOTE_USER"] = ; // same
	}

    environments["SERVER_PROTOCOL"] = "HTTP/1.1";
    environments["SERVER_SOFTWARE"] = "webserv";
    //environments["HTTP_X-Secret-Header-For-Test"] = "1";
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
    int status, child , tmp_fd;

	tmp_fd = open("./tmp", O_RDWR | O_CREAT, S_IRWXU);
    if (tmp_fd < 0 || pipe(fd) < 0 || (child = fork()) < 0)
        throw (std::runtime_error(strerror(errno)));
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
	else
    {
	    close(fd[0]);
	    //dup2(fd[1],1);
	    write(fd[1], body.c_str(), body.size());
	    close(fd[1]);
	    waitpid(child, &status, 0);
	    readFromCGI();
    }
	return (1);
}

int     CGI::readFromCGI()
{
    int fd = open("./tmp", O_RDONLY);
    long size = findFileSize(fd);

    std::cout << "HELLO from cgi" << std::endl;
    if (size < 0 || !(buff = (char*)malloc(sizeof(char) * (size + 1))))
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







