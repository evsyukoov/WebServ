//
// Created by Casie Carl on 12/15/20.
//

#include "CGI.hpp"
#include <array>

CGI::CGI(const ServConf &_servConf, input &_in) : in(_in), servConf(_servConf) 
{
	timeval tv;

	gettimeofday(&tv, nullptr);
	tmpFile = "ws_" + std::to_string(tv.tv_sec * 1000000 + tv.tv_usec);
	filename = trimAfter(in.root, '/');
	extension = '.' + trimAfter(filename, '.');
	env = nullptr;
	buff = nullptr;
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
	delete buff;
}

int        CGI::initARGS()
{
	if (in.interptretator.empty())
	{
		args[0] = const_cast<char*>(in.scrypt.c_str());
		args[1] = nullptr;
	}
	else
	{
		args[0] = const_cast<char*>(in.interptretator.c_str());
		args[1] = const_cast<char*>(in.scrypt.c_str());
		args[2] = nullptr;
	}
	return (1);
}

void		CGI::setUriAttributes()
{
	std::string	&uri = (*in.requestMap)["Location"];
	size_t		delim = uri.find('?');

	if (delim != uri.npos)
		envMap["QUERY_STRING"] = uri.substr(uri.find('?'), uri.size());
	if ((*in.requestMap).count("lol"))
	{
		envMap["PATH_INFO"] = "this%2eis%2epath%3binfo"; // non-null
		envMap["REQUEST_URI"] = "/";
		envMap["PATH_TRANSLATED"] = "/";
	}
	else
		envMap["PATH_INFO"] = "/"; // null
}

void		CGI::setHttpHeaders()
{
	std::map<std::string, std::string>::iterator it = (*in.requestMap).begin();
	static std::array<std::string, 4> extras = { "body", "method", "location", "protocol" };
	std::string	httpKey;

	while (it != (*in.requestMap).end())
	{
		if (std::find(extras.begin(), extras.end(), it->first) == extras.end())
		{
			httpKey = it->first;
			std::transform(httpKey.begin(), httpKey.end(), httpKey.begin(), ::toupper);
			std::replace(httpKey.begin(), httpKey.end(), '-', '_');

			envMap["HTTP_" + httpKey] = it->second;
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
	if (in.requestMap->count("Authorization"))
	{
		std::string	&auth = (*in.requestMap)["Authorization"];
		size_t		delim = auth.find(' ');

		envMap["AUTH_TYPE"] = auth.substr(0, delim - 1);
		envMap["REMOTE_USER"] = auth.substr(delim + 1, auth.size());
		envMap["REMOTE_IDENT"] = envMap["REMOTE_USER"];
	}
	envMap["CONTENT_LENGTH"] = (*in.requestMap)["Content-Length"];
	envMap["CONTENT_TYPE"] = (*in.requestMap)["Content-Type"];
	envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	envMap["REMOTE_ADDR"] = in.remote_addr;
	envMap["REQUEST_METHOD"] = (*in.requestMap)["method"];
	envMap["SERVER_NAME"] = servConf.getServerName();
	envMap["SERVER_PORT"] = std::to_string(servConf.getPort());
	envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	envMap["SERVER_SOFTWARE"] = "webserv";
}

int     CGI::mapToEnv()
{
	int			i = 0;
	std::string	envir;
	std::map<std::string, std::string>::iterator it;

	if (!(env = (char**)malloc(sizeof(char*) * (envMap.size() + 1))))
			throw(std::runtime_error(strerror(errno)));
	for(it = envMap.begin(); it != envMap.end(); it++)
	{
		envir = (it->first + "=" + it->second);
		if (!(env[i] = ft_strdup(envir.c_str())))
			throw(std::runtime_error(strerror(errno)));
		i++;
	}
	env[i] = nullptr;
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
		if (execve(args[0], args, env) == -1)
		{
			std::cerr << "Problems with execve: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(fd[0]);
		write(fd[1], (*in.requestMap)["body"].c_str(), (*in.requestMap)["body"].size());
		close(fd[1]);
		waitpid(child, &status, 0);
		readFromCGI();
	}
}

std::pair<std::string, std::string> splitPair(std::string const &str, std::string const &c)
{
	size_t delim = str.find(c);

    if (delim == str.npos)
		return (std::pair<std::string, std::string>(str, str));
	return (std::pair<std::string, std::string>(str.substr(0, delim), str.substr(delim + c.size(), str.size())));
}

void	CGI::createResponseMap()
{
	int		fd = open(tmpFile.c_str(), O_RDONLY);
	size_t	headlen = 0;
	std::pair<std::string, std::string> pr;
	std::string str;

	while (lseek_next_line(fd, str) > 0 && str[0] != '\r')
	{
		PRINT(str)
		headlen += str.size() + 1;
		str.pop_back();
		responseMap.insert(splitPair(str, ": "));
	}
	responseMap.insert(std::pair<std::string, std::string>("#file", tmpFile));
	responseMap.insert(std::pair<std::string, std::string>("#lseek", std::to_string(headlen += 2)));
	lseek(fd, headlen, SEEK_SET);
	close(fd);
}

void    CGI::readFromCGI()
{
	/*
	std::ifstream ifs;
	std::ostringstream oss;
	ifs.open("./tmp");

	oss << ifs.rdbuf();
	response = oss.str();
	*/
	int		fd = open(tmpFile.c_str(), O_RDONLY);
	long	size = findFileSize(fd);

	if (size < 0 || fd < 0)
		throw (std::runtime_error(strerror(errno)));
	createResponseMap();
}

const std::string &CGI::getResponse() const
{
	return response;
}

std::map<std::string, std::string> CGI::getRespMap() const
{
	return responseMap;
}
