//
// Created by Filipp Ewseew on 1/23/21.
//

#include "Server.hpp"

#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"
#include "Debug.hpp"
#include "Threadpool.hpp"

int Server::listen(const ServConf &servConf) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);

	if (listener < 0)
		return (error("sock error"));
	int optval = 1;
	//если сокет уже был открыт
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0 && setsockopt(listener, SOL_SOCKET, SO_NOSIGPIPE,  &optval, sizeof(int)) < 0) {
		close(listener);
		return (0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(servConf.getPort());
	addr.sin_addr.s_addr = inet_addr(host.c_str());


	if (::bind(listener, ((struct sockaddr*)&addr), sizeof(addr)) != 0) {
		close(listener);
		return (0);
	}
	if (::listen(listener, SOMAXCONN) != 0)
	{
		close(listener);
		return (0);
	}
	return (listener);
}


int     set_nonblock(int fd)
{
	return (fcntl(fd, F_SETFL | O_NONBLOCK));
}

Server::Server(input &in, const Config &config)
: AServer(in, config), Threadpool<Worker, Client *>(255)
{
	for (int i = 0; i < 4; i++)
		proletariat.push_back(Worker(in, config));
	setWorkers(proletariat);
	initThreadpool();
	host = "127.0.0.1";
	this->in = in;
	this->config = config;
	FD_ZERO(&read_set);
}

int Server::openServers()
{
	std::list<ServConf>::const_iterator it;
	for(it = config.getConfig().begin(); it != config.getConfig().end(); it++)
	{
		int listener;
		if (!(listener = listen(*it))) {
			std::cerr << "Port number " << it->getPort() << " is unavailable!" << std::endl;
			continue;
		}
		set_nonblock(listener);
		container[listener] = nullptr;
		PRINT("Server " << it->getServerName() << ":" << it->getPort()
		                << " started.");
	}
	if (container.empty())
		return (0);
	return (1);
}

int     Server::run()
{
	if (!openServers())
		return (error("No available ports. Stop server"));
	while (true)
		servLoop();
	return (1);
}

//отладочная печать клиентов

void        print(const std::list<Client*> &clients)
{
	std::cout << RED << "AVAILABLE CLIENTS: ";
	std::list<Client*>::const_iterator it;
	for( it = clients.begin(); it != clients.end(); it++)
		std::cout << (*it)->getClientSock() << " " ;
	std::cout << RESET << std::endl;
}

void    Server::acceptConnection(int sockFd)
{

	sockaddr_in sAddr;
	socklen_t sLen = sizeof(sAddr);
	int client_sock;

	client_sock = accept(sockFd, reinterpret_cast<sockaddr *>(&sAddr), &sLen);
	if (client_sock < 0)
		return ;
	set_nonblock(client_sock);
	Client *client = new Client(client_sock, sAddr);

	addTask(client);
}

void	Server::doTheJob()
{
	std::map<int, Client *>::iterator it;
	for (it = container.begin(); it != container.end(); it++)
	{
		if (FD_ISSET(it->first, &read_set))
			acceptConnection(it->first);
	}
}

Server::~Server() {
}

int Server::error(std::string msg) {
	std::cerr << "Server error: " << msg << std::endl;
	return (0);
}