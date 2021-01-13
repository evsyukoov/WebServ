//
// Created by Casie Carl on 12/15/20.
//



#include <fstream>
#include "CGI.hpp"
#include <algorithm>
#include <array>

CGI::CGI(const ServConf &_servConf, input &_in) : in(_in), servConf(_servConf) 
{
	timeval tv;

	gettimeofday(&tv, NULL);
	tmpFile = "ws_" + std::to_string(tv.tv_sec * 1000000 + tv.tv_usec);
	filename = in.root.substr(in.root.rfind('/') + 1, in.root.npos);
	extension = filename.substr(filename.rfind('.'), filename.size());

	env = NULL;
	buff = NULL;
	initEnvironments();
	initARGS();
	mapToEnv();
}

CGI::~CGI()
{
	if (env)
	{
		for (int i = 0; env[i]; ++i)
			free(env[i]);
		free(env);
	}
	if (buff)
		delete buff;
}

int        CGI::initARGS()
{
	if (in.interptretator.empty())
	{
		args[0] = const_cast<char*>(in.scrypt.c_str());
		args[1] = NULL;
	}
	else
	{
		args[0] = const_cast<char*>(in.interptretator.c_str());
		args[1] = const_cast<char*>(in.scrypt.c_str());
		args[2] = 0;
	}
	return (1);
}

void		CGI::setUriAttributes()
{
	std::string	&uri = (*in.reqestMap)["Location"];
	size_t		delim = uri.find('?');

	if (delim != uri.npos)
		environments["QUERY_STRING"] = uri.substr(uri.find('?'), uri.size());
	if ((*in.reqestMap).count("lol"))
	{
		environments["PATH_INFO"] = "this%2eis%2epath%3binfo"; // nonvoid
		environments["REQUEST_URI"] = "/";
		environments["PATH_TRANSLATED"] = "/";
	}
	else
		environments["PATH_INFO"] = "/"; // void
}

void		CGI::setHttpHeaders()
{
	std::map<std::string, std::string>::iterator it = (*in.reqestMap).begin();
	static std::array<std::string, 4> extras = { "body", "method", "location", "protocol" };
	std::string	httpKey;
	int			i;

	while (it != (*in.reqestMap).end())
	{
		if (std::find(extras.begin(), extras.end(), it->first) == extras.end())
		{
			httpKey = it->first;
			std::transform(httpKey.begin(), httpKey.end(), httpKey.begin(), ::toupper);
			std::replace(httpKey.begin(), httpKey.end(), '-', '_');

			environments["HTTP_" + httpKey] = it->second;
		}
		++it;
	}
}

void        CGI::initEnvironments()
{
	/*
	**	https://tools.ietf.org/html/rfc3875#section-4.1.1	
	*/

	setUriAttributes();
	setHttpHeaders();
	if (in.reqestMap->count("Authorization"))
	{
		std::string	&auth = (*in.reqestMap)["Aurthorization"];
		size_t		delim = auth.find(' ');
		environments["AUTH_TYPE"] = auth.substr(0, delim - 1);
		environments["REMOTE_USER"] = auth.substr(delim + 1, auth.size());
		environments["REMOTE_IDENT"] = environments["REMOTE_USER"];
	}
	environments["CONTENT_LENGTH"] = std::to_string((*in.reqestMap)["body"].size());//(*in.reqestMap)["Content-Length"];
	environments["CONTENT_TYPE"] = (*in.reqestMap)["Content-Type"];
	environments["GATEWAY_INTERFACE"] = "CGI/1.1";
	environments["REMOTE_ADDR"] = in.remote_addr;
	environments["REQUEST_METHOD"] = (*in.reqestMap)["method"];
	environments["SERVER_NAME"] = servConf.getServerName();
	environments["SERVER_PORT"] = std::to_string(servConf.getPort());
	environments["SERVER_PROTOCOL"] = "HTTP/1.1";
	environments["SERVER_SOFTWARE"] = "webserv";
}

int     CGI::mapToEnv()
{
	int i = 0;
	std::string envir;
	std::map<std::string, std::string>::iterator it;

	if (!(env = (char**)malloc(sizeof(char*) * (environments.size() + 1))))
		return (0);
	for(it = environments.begin(); it != environments.end(); it++)
	{
		envir = (it->first + "=" + it->second);
		env[i] = ft_strdup(envir.c_str());
		i++;
	}
	env[i] = NULL;
	return (1);
}

void	CGI::run()
{
	int fd[2], status, tmp_fd, child;

	status = pipe(fd);
	tmp_fd = open(tmpFile.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (status < 0 || tmp_fd < 0 || (child = fork ()) < 0)
		throw (std::runtime_error(strerror(errno)));
	if (child == 0)
	{
		close(fd[1]); //ничего не пишем
		//заменяем stdout дочернего процесса на дескриптор временного файла
		dup2(tmp_fd, 1);
		//читать запрос будем от родителя
		dup2(fd[0], 0);
		close(tmp_fd);
		close(fd[0]);
		if (execve(args[0],  args, env) == -1)
		{
			std::cerr << "Problems with execve: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(fd[0]);
		write(fd[1], (*in.reqestMap)["body"].c_str(), (*in.reqestMap)["body"].size());
		close(fd[1]);
		waitpid(child, &status, 0);
		readFromCGI();
	}
}

void     CGI::readFromCGI()
{
	/*
	std::ifstream ifs;
	std::ostringstream oss;
	ifs.open("./tmp");

	oss << ifs.rdbuf();
	response = oss.str();
	*/
	int		n;
	int		fd = open(tmpFile.c_str(), O_RDONLY);
	long	size = findFileSize(fd);

	if (size < 0 || fd < 0)
		throw (std::runtime_error(strerror(errno)));
	buff = new char[size + 1];
	n = read(fd, buff, size);
	buff[n] = '\0';
	response = buff;
	unlink(tmpFile.c_str());
}

const std::string &CGI::getResponse() const {
	return response;
}
