//
// Created by Casie Carl on 12/15/20.
//

#include "CGI.hpp"
#include <array>

CGI::CGI(const ServConf &_servConf, const Location &location, input &in) : in(in), servConf(_servConf)
{
	interpretator.clear();
    this->location = location;
	timeval tv;

	gettimeofday(&tv, nullptr);
	tmpFile = "/tmp/ws_" + std::to_string(tv.tv_sec * 1000000 + tv.tv_usec);
	filename = trimAfter(in.root, '/');
	extension = '.' + trimAfter(filename, '.');
	args[1] = nullptr;
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
	findInterpretator();
    if (interpretator.empty())
    { 
		args[0] = const_cast<char *>(location.getCgiScrypt().c_str());
        args[1] = const_cast<char *>(in.root.c_str());
        args[2] = nullptr;
    }
    else
    {
        args[0] = const_cast<char *>(interpretator.c_str());
        args[1] = const_cast<char *>(location.getCgiScrypt().c_str());
        args[2] = const_cast<char *>(in.root.c_str());
        args[3] = nullptr;
    }
    std::cout << "scrypt: " << args[1] << std::endl;
	return (1);
}

void		CGI::setUriAttributes()
{
	std::string	&uri = (*in.requestMap)["Location"];
	size_t		q_pos = uri.find('?');
	size_t		slash_pos = uri.rfind('/');

	//@TOOD double check uri variables
	if (q_pos != std::string::npos)
		envMap["QUERY_STRING"] = uri.substr(q_pos + 1, uri.size());
	if (slash_pos != std::string::npos && q_pos < slash_pos)
	{
		envMap["PATH_INFO"] = uri.substr(slash_pos + 1, uri.size());
		envMap["REQUEST_URI"] = uri;
		envMap["PATH_TRANSLATED"] = location.getRoot() + envMap["PATH_INFO"];
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
	//@TODO double check auth
	if (in.requestMap->count("Authorization"))
	{
		std::string	&auth = (*in.requestMap)["Authorization"];
		std::pair<std::string, std::string> credentials = splitPair(auth, ' ');

		envMap["AUTH_TYPE"] = credentials.first;
		envMap["REMOTE_USER"] = credentials.second;
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

int	CGI::run()
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
            exit(2);
		exit(1);
	}
	else
	{
		close(fd[0]);
		std::cout << "reqMap: " << (*in.requestMap)["body"] << std::endl;
		write(fd[1], (*in.requestMap)["body"].c_str(), (*in.requestMap)["body"].size());
		close(fd[1]);
		waitpid(child, &status, 0);
		if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 2)
                return (0);
		}
		readFromCGI();
	}
	return (1);
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
	int		rv = 0;
	size_t	headlen = 0;
	std::pair<std::string, std::string> pr;
	std::string str;

	/* get response line */

	lseek_next_line(fd, str);
		//@TODO INTERNAL SERVER ERROR
	if (!std::strncmp(str.c_str(), "Status: ", 8) && str[str.size() - 1] == '\r')
	{
		/* Response line found! Adding headers */

		lseek(fd, 0, SEEK_SET);
		while ((rv = lseek_next_line(fd, str)) > 0 && str[0] != '\r')
		{
			headlen += str.size() + 1;
			if (!str.empty())
				str.pop_back();
			responseMap.insert(splitPair(str, ": "));
		}
		//if (rv < 0)
			// INTERNAL SERVER ERROR
	}
	responseMap.insert(std::pair<std::string, std::string>("#file", tmpFile));
	responseMap.insert(std::pair<std::string, std::string>("#lseek", std::to_string(headlen += 2)));
	//lseek(fd, headlen, SEEK_SET);
	close(fd);
}

void    CGI::readFromCGI()
{
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

void CGI::findInterpretator()
{
	static const std::string hashTable[2][6] = {
		{ ".php", ".py", ".pl", ".rb", ".rbw", "" },
		{ "php", "python", "perl", "ruby", "ruby", "" }
	};

	std::string scrypt_extension = location.getCgiScrypt();
	size_t dot_pos = scrypt_extension.rfind('.');
	if (dot_pos == std::string::npos)
		return ;
	scrypt_extension = scrypt_extension.substr(dot_pos);

	args[0] = const_cast<char *>("/usr/bin/whereis");
	for (int i = 0; !hashTable[0][i].empty(); ++i)
	{
		if (scrypt_extension == hashTable[0][i])
		{
			args[1] = const_cast<char *>(hashTable[1][i].c_str());
			break ;
		}
	}
	if (args[1] == nullptr)
		return ;
	int		status;
	pid_t	child;
	int		fd[2];
	char	read_buff[1024] = "";

	
	if (pipe(fd) < 0 || (child = fork()) < 0)
		throw (std::runtime_error(strerror(errno)));
	if (child == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		if (execve(args[0], args, nullptr) == -1)
		{
			close(fd[1]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(fd[1]);
		waitpid(child, &status, 0);
		int n = read(fd[0], read_buff, sizeof(read_buff) - 1);
		close(fd[0]);
		if (n < 0)
			return ;
		read_buff[n - 1] = '\0';
		interpretator = read_buff;
	}
}
