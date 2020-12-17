//
// Created by Casie Carl on 12/15/20.
//


#include <iostream>
#include "CGI.hpp"

CGI::CGI(char *childProgram, char **env, char **args, char *request) : child_program(childProgram), env(env), args(args), request(request)
{}

int	CGI::run_child()
{
	int child = fork();
	if (child < 0)
		std::cerr << "Problems with fork" << std::endl;
	else if (child == 0)
	{
		if (!(execve(child_program, args, env)))
		{
			std::cerr << "Problems with execve" << std::endl;
			exit(EXIT_FAILURE);
		}
		exit (EXIT_SUCCESS);
	}
	return (child);
}

void 	CGI::run_parent()
{
	int fdStdIn[2];
	int fdStdOut[2];
	std::cout << "Begin reading" << std::endl;
	if (pipe(fdStdIn) != 0 || pipe(fdStdOut) != 0)
	{
		std::cerr << "Error pipe!" << std::endl;
		return ;
	}

	int		fdOldIn = dup(0);
	int 	fdOldOut = dup(1);

	dup2(fdStdOut[1], 1);
	dup2(fdStdIn[0], 0);
//
	close(fdStdOut[1]);
	close(fdStdIn[0]);

	int pid = run_child();

	dup2(fdOldIn, 1);
	dup2(fdOldOut, 0);
	close(fdOldIn);
	close(fdOldOut);

	//пишем в трубу(на stdin дочернего процесса)
	write(fdStdIn[1], request, ft_strlen(request));
	int nread;
	char buff[100] = "";
	int status;
	std::cout << "Begin reading 2" << std::endl;
	while (1)
	{
		read(fdStdOut[0], buff, 100);
		if (waitpid(pid, &status, WNOHANG) < 0)
			break ;
	}
	std::cout << buff[0] << std::endl;
	std::cout << buff[1] << std::endl;
}
