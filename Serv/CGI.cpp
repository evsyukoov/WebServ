//
// Created by Casie Carl on 12/15/20.
//



#include <fstream>
#include "CGI.hpp"

CGI::CGI(char *childProgram, char **env, char **args, const std::string &request) :
child_program(childProgram), env(env), args(args), request(request)
{}


void        CGI::initEnvironments()
{
    environments["AUTH_TYPE"] = "Basic";
    //environments["CONTENT_LENGTH"] = ;
   // environments["CONTENT_TYPE"] = ;
   environments["GATEWAY_INTERFACE"] = "CGI/1.1";
}

int	CGI::run()
{
    int fd[2];
    int status;
    if (pipe(fd) < 0)
    {
        std::cerr << "Pipe error" << std::endl;
        return (0);
    }
	int child = fork();
	int tmp_fd = open("./tmp", O_RDWR | O_CREAT, S_IRWXU);
	if (tmp_fd < 0)
    {
	    std::cerr << "Error with tmp file on CGI" << std::endl;
	    return (0);
    }
	if (child < 0) {
        std::cerr << "Problems with fork" << std::endl;
        return (0);
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
		if (!(execve(child_program, args, env)))
		{
			std::cerr << "Problems with execve" << std::endl;
			exit(EXIT_FAILURE);
		}
		exit (EXIT_SUCCESS);
	}
	//родитель
	else
    {
	    close(fd[0]);
	    write(fd[1], request.c_str(), request.size());
	    close(fd[1]);
	    wait(&status);
	    readFromCGI();
    }
	return (1);
}

int     CGI::readFromCGI()
{
    int fd = open("./tmp", O_RDONLY);
    if (fd < 0)
    {
        std::cerr << "Error open tmp file from CGI scrypt" << std::endl;
        return (0);
    }
    long size = findFileSize(fd);
    if (size < 0)
    {
        std::cerr << "Error reading from tmp file" << std::endl;
        return (0);
    }

    char buff[size];
    int n = read(fd, buff, size);
    buff[n] = '\0';
    response = buff;
    unlink("./tmp");
    return (1);
}

const std::string &CGI::getResponse() const {
    return response;
}

