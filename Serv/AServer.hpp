//
// Created by Filipp Ewseew on 1/21/21.
//

#ifndef ASERVER_HPP
# define ASERVER_HPP

#include <map>
#include <vector>
#include <fcntl.h>
#include "Config.hpp"
#include "HTTP_BONUS.hpp"
#include "Client.hpp"

class AServer
{
 protected:

	int    max;
	fd_set read_set;
	fd_set write_set;
	timeval timeout;
	input  in;
	Config config;

	std::map<int, Client *>     container;

	virtual void iterateReadSet()
	{
		std::map<int, Client *>::iterator it;
		for (it = container.begin(); it != container.end(); ++it)
		{
			FD_SET(it->first, &read_set);
			max = std::max(max, it->first);
		};
	}

	virtual void iterateWriteSet() = 0;

	void resetFdSets()
	{
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		max = 0;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
	};

	void listenSockets()
	{
		int ret = select(max + 1, &read_set, &write_set, nullptr, &timeout);
		if (ret < 0)
			throw (std::runtime_error(strerror(errno)));
	};

 public:

	AServer(input const &inp, Config const &conf)
		: in(inp), config(conf) {};

	AServer(const AServer &copy)
	{
		operator=(copy);
	}

	AServer &operator=(const AServer &copy)
	{
		max = copy.max;
		read_set = copy.read_set;
		write_set = copy.write_set;
		in = copy.in;
		config = copy.config;
		container = copy.container;
		return (*this);
	}

	virtual ~AServer()
	{
		std::map<int, Client *>::iterator it;
		for (it = container.begin(); it != container.end(); it++)
			close(it->first);
		container.clear();
	}

	virtual void doTheJob() = 0;

	void servLoop()
	{
		//ключ-сокет клиента  - value - конфиг сервера на котором коннект
		resetFdSets();
		iterateReadSet();
		iterateWriteSet();
		listenSockets();
		//  ждем коннекта или готовности к чтению
		//  бежим по всем soketam, смотрим на каком событие
		doTheJob();
	}
};

#endif //WS_SERV_ASERVER_HPP_
