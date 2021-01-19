//
// Created by Casie Carl on 12/18/20.
//

#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"
#include "Debug.hpp"

int Server::listen(const ServConf &servConf) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);

	std::cout << "Socket: " << listener << std::endl;
	if (listener < 0)
		return (error("sock error"));
	int optval = 1;
	//если сокет уже был открыт
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
		close(listener);
		return (0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(servConf.getPort());
	addr.sin_addr.s_addr = inet_addr(servConf.getServerName().c_str());

	if (::bind(listener, ((struct sockaddr*)&addr), sizeof(addr)) != 0) {
		close(listener);
		return (0);
	}
	if (::listen(listener, SOMAXCONN) != 0) {
		close(listener);
		return (0);
	}
	return (listener);
}


int     set_nonblock(int fd)
{
    //int flags = fcntl(fd, F_GETFL);
    return (fcntl(fd, F_SETFL | O_NONBLOCK));
}


int   Server::receiveData(int client_sock, std::string &str)
{
    char recieve[WS_BUFFER_SIZE + 1];

    int len = read(client_sock, recieve, WS_BUFFER_SIZE);
	if (len <= 0)
		return (len);
    recieve[len] = '\0';
    str = recieve;
    //std::cout << BLUE << "recieve: " << str  << RESET << std::endl;
    return (1);
}



Server::Server(input &in,const Config &config)
{
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
        std::pair<int , ServConf> pair = std::make_pair(listener, *it);
        servers.insert(pair);
    }
    if (servers.empty())
    	return (0);
    return (1);
}

int     Server::run(HTTP &http)
{
    if (!openServers())
        return (error("No available ports. Stop server"));
    servLoop(http);
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

void Server::resetFdSets()
{
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);

    max = 0;
	std::map<int, ServConf>::iterator it;
	for(it = servers.begin(); it != servers.end(); it++)
	{
		FD_SET((*it).first, &read_set);
		max = std::max(max, (*it).first);
	}
	std::list<Client*>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); ++iter)
    {
	    if ((*iter)->getState() == WRITING || (*iter)->getState() == WRITING_BODY)
		    FD_SET((*iter)->getClientSock(), &write_set);
	    else
		    FD_SET((*iter)->getClientSock(), &read_set);
	    max = std::max(max, (*iter)->getClientSock());
    }
}

void    Server::acceptConnection(int sockFd, ServConf &serv)
{

	sockaddr_in sAddr;
	socklen_t sLen = sizeof(sAddr);
	int client_sock;

	client_sock = accept(sockFd, reinterpret_cast<sockaddr *>(&sAddr), &sLen);
	if (client_sock < 0)
		return ;
	set_nonblock(client_sock);
	Client *client = new Client(client_sock, serv, sAddr);
	clients.push_back(client);
}

_Noreturn void Server::servLoop(HTTP &http)
{
	//ключ-сокет клиента  - value - конфиг сервера на котором коннект
	int ret;
	std::map<int, ServConf>::iterator it;

	while (true)
	{
		resetFdSets();
		//  ждем коннекта или готовности к чтению
		ret = select(max + 1, &read_set, &write_set, nullptr, nullptr);
		if (ret < 0)
			throw (std::runtime_error(strerror(errno)));
		//  бежим по всем серверам, смотрим на каком событие
		for (it = servers.begin(); it != servers.end(); it++)
		{
			if (FD_ISSET(it->first, &read_set))
				acceptConnection(it->first, it->second);
		}
		std::vector<char *> requests = readRequests();
		sendToAllClients(requests, http);
	}
}

std::vector<char*>      Server::readRequests()
{
	std::vector<char*> requests;
    std::string req;

    std::list<Client*>::iterator it = clients.begin();
    std::list<Client*>::iterator ite = clients.end();
	while (it != ite)
	{
        std::string data;
        //смотрим что кто то из клиентов что-то отправил(сервер готов читать)
		if (FD_ISSET((*it)->getClientSock(), &read_set))
		{
		    int ret = receiveData((*it)->getClientSock(), data);

			//кто-то отключился
			if (ret == 0)
			{
				close((*it)->getClientSock());
				delete (*it);
				it = clients.erase(it);
			}
			//если не отключился значит готов принять ответ
			//смотрим что запрос полный
			else if (ret > 0)
                (*it)->findState(data);
		}
		it++;
	}
	return (requests);
}

int    sendBodySegment(Response *rs)
{
	StringResponse *sr = dynamic_cast<StringResponse *>(rs);

	if (sr != nullptr)
		return (sr->sendPiece());
	return (dynamic_cast<FileResponse *>(rs)->sendPiece());
}

void	Server::sendToAllClients(std::vector<char*> requests, HTTP &http)
{
    std::list<Client*>::iterator it = clients.begin();
    std::list<Client*>::iterator ite = clients.end();
    Response *r;
	while (it != ite)
    {
		if (FD_ISSET((*it)->getClientSock(), &write_set))
		{
			if ((*it)->getState() == WRITING)
			{
				in.remote_addr = (*it)->getRemoteAddr();
				http.setFields((*it)->getClientSock(), (*it)->getBody(), (*it)->getServConf(), in, (*it)->getReqMap());
				http.manager();
				r = http.getResponse();
				(*it)->setResponse(r);
				(*it)->getResponse()->sendHeader();
			}
			else if ((*it)->getState() == WRITING_BODY)
			{
				if (!sendBodySegment((*it)->getResponse()))
					(*it)->clear();
			}
		}
		it++;
	}
	//@TODO unused parameter requests
    (void)requests;
}

Server::~Server() {
    for (std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++)
        close(it->first);
    servers.clear();
}

int Server::error(std::string msg) {
	std::cerr << "Server error: " << msg << std::endl;
	return (0);
}


