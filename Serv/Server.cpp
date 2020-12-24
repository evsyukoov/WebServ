//
// Created by Casie Carl on 12/18/20.
//

#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

int Server::listen(const ServConf &servConf) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
		return (error("sock error"));
	int optval = 1;
	//если сокет уже был открыт
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		return error("setsockopt error");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(servConf.getPort());
	addr.sin_addr.s_addr = inet_addr(servConf.getServerName().c_str());

	if (::bind(listener, ((struct sockaddr*)&addr), sizeof(addr)) != 0)
		return (error("bind failed"));
	if (::listen(listener, SOMAXCONN) != 0)
		return (error("listen error"));
	return (listener);
}


int     set_nonblock(int fd)
{
    //int flags = fcntl(fd, F_GETFL);
    return (fcntl(fd, F_SETFL | O_NONBLOCK));
}


char*   Server::receiveData(int client_sock)
{
    char recieve[4096];

    std::cout << "Wait for reading request from client" << std::endl;
    int len = read(client_sock, recieve, 4096);
    if (len == 0)
        return (nullptr);
    recieve[len] = '\0';
    return (recieve);
}



Server::Server(const Config &config)
{
    this->config = config;
    FD_ZERO(&read_set);
}

int Server::openServers()
{
    for(std::list<ServConf>::const_iterator it = config.getConfig().begin(); it != config.getConfig().end(); it++)
    {
    	int listener;
        if ((listener = listen(*it)) < 0)
            return (0);
        set_nonblock(listener);
        std::pair<int , ServConf> pair = std::make_pair(listener, *it);
        servers.insert(pair);
    }
    return (1);
}

void     Server::initReadSet()
{
    for(std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++) {
        FD_SET((*it).first, &read_set);
    }
}

int     Server::run()
{
    if (openServers() == -1)
        return (-1);
    servLoop();
    return (1);
}

int Server::servLoop() {
    //ключ-сокет клиента  - value - конфиг сервера на котором коннект
	std::map<int, ServConf> clients;
	while (true) {
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		initReadSet();
		for (std::map<int, ServConf>::iterator it = clients.begin(); it != clients.end(); it++) {
			FD_SET((*it).first, &read_set);
		}

		int max;
		if (!clients.empty())
		    max = std::max((--servers.end())->first, (--clients.end())->first);
		else
		    max = (--servers.end())->first;

        //ждем коннекта или готовности к чтению
		std::cout << "select block" << std::endl;
		select(max + 1, &read_set, NULL, NULL, NULL);
		std::cout << "select unblock, max: " << max << std::endl;
		//бежим по всем серверам, смотрим на каком событие
		for (std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++) {
			// произошел коннект на n-ом сервере
			if (FD_ISSET((*it).first, &read_set)) {

				int client_sock = accept((*it).first, NULL, NULL);
				set_nonblock(client_sock);
				std::pair<int, ServConf> pair = std::make_pair(client_sock, (*it).second);
				clients.insert(pair);
				std::cout << "listener =  " << (*it).first << " client_sock = " << client_sock << std::endl;
			}
		}
		std::vector<char*> requests = readRequests(clients);
		sendToAllClients(requests, clients);
		requests.clear();
	}
	return (1);
}

std::vector<char*>      Server::readRequests(std::map<int, ServConf> &clients)
{
	std::vector<char*> requests;
	HTTP http;
	//смотрим что кто то из клиентов что-то отправил(сервер готов читать)
    std::map<int, ServConf>::iterator it = clients.begin();
    std::map<int, ServConf>::iterator ite = clients.end();
    int i = 0;
	while (it != ite)
	{
        std::cout << "i1: " << i << std::endl;
		if (FD_ISSET(it->first, &read_set))
		{
            char *req;
			//кто-то отключился
			if (!(req = receiveData(it->first)))
			{
				close(it->first);
				it = clients.erase(it);
			}
			//если не отключился значит готов принять ответ
			else
			{
			    std::cout << "REquest prinyat!" << std::endl;
			    //std::cout << "Write set on" << std::endl;
				FD_SET(it->first, &write_set);
				requests.push_back(req);
				it++;
			}
		}
		else
		    it++;
		i++;
	}
	return (requests);
}

void	Server::sendToAllClients(std::vector<char*> requests, std::map<int, ServConf> clients)
{
	int i = 0;
	HTTP http;
	for(std::map<int, ServConf>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (FD_ISSET(it->first, &write_set))
		{
		    http.setFields(it->first, requests[i], it->second);
			http.manager();
		//	std::cout << "out" << std::endl;
		//	std::string response = http.getResponce();
		//	send(it->first, response.c_str(), response.size(), 0);
			shutdown(it->first, SHUT_RDWR);
			i++;
		}
	}

}

Server::~Server() {
    for (std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++)
        close(it->first);
    servers.clear();
}


