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

	//std::cout << "Socket: " << listener << std::endl;
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
    return (1);
}



Server::Server(input &in,const Config &config) : AServer(in, config)
{
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
        container[listener] = 0;
    }
    if (container.empty())
    	return (0);
    return (1);
}

int     Server::run(HTTP &http)
{
	(void)http;
    if (!openServers())
        return (error("No available ports. Stop server"));
    PRINT("HERE")
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
	clients.push_back(client);
	std::cout << "Client " << client_sock << " connected to server" << std::endl;
}

/*
void Server::servLoop(HTTP &http)
{
	//ключ-сокет клиента  - value - конфиг сервера на котором коннект
	int ret;
	std::map<int, Client*>::iterator it;

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
			if (FD_ISSET(it->first, &read_set)) {
                acceptConnection(it->first);
                servers[it->first] = clients.back();
            }
		}
		readRequests();
		sendToAllClients(http);
	}
}*/

void      Server::readRequests()
{
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
                std::cout << "Client " << (*it)->getClientSock() << " disconnected" << std::endl;
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
}

int    sendBodySegment(Response *rs)
{
	StringResponse *sr = dynamic_cast<StringResponse *>(rs);

	if (sr != nullptr)
		return (sr->sendPiece());
	return (dynamic_cast<FileResponse *>(rs)->sendPiece());
}

int    Server::findServerName(const std::string &server_name, Client *client)
{
    size_t pos = server_name.find(':');
    std::string host_header = server_name.substr(0, pos);
    std::string port_header = server_name.substr(pos + 1);
    int port = std::stoi(port_header);
    std::list<ServConf> serv = config.getConfig();
    //ищем совпадение на server_name
    for(std::list<ServConf>::iterator it = serv.begin(); it != serv.end(); it++)
    {
        if (port == it->getPort() && host_header == it->getServerName()) {
            client->setServConf(*it);
            return (1);
        }
    }
    //если не нашли
    if (host_header != "localhost" && host_header != "127.0.0.1")
        return (0);
    //иначе ищем первое совпадение по порту
    for(std::list<ServConf>::iterator it = serv.begin(); it != serv.end(); it++)
    {
        if (port == it->getPort())
        {
            client->setServConf(*it);
            return (1);
        }
    }
    return (1);
}

void    printReqMap(std::map<std::string, std::string> req_map)
{
    for(std::map<std::string, std::string>::iterator it = req_map.begin(); it != req_map.end(); it++)
        std::cout << RED << it->first << " : " << it->second << std::endl;
    std::cout << RESET << std::endl;
}

void	Server::sendToAllClients()
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
				std::map<std::string, std::string> map((*it)->getReqMap());
				HTTP http;

				//отдаем пустую мапу, если не нашли host, после обработки получим 400
                if (map.count("Host") == 0 || !(findServerName(map["Host"], *it)))
                    (*it)->clearRequest();
                //printReqMap(map);
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
}

void Server::iterateWriteSet() {
	std::list<Client *>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getState() == WRITING || (*it)->getState() == WRITING_BODY)
			FD_SET((*it)->getClientSock(), &write_set);
		FD_SET((*it)->getClientSock(), &read_set);
		max = std::max(max, (*it)->getClientSock());
	}
}


void Server::doTheJob()
{
	std::map<int, Client *>::iterator it;

	for (it = container.begin(); it != container.end(); it++)
	{
		if (FD_ISSET(it->first, &read_set)) {
			acceptConnection(it->first);
			container[it->first] = clients.back();
		}
	}
	readRequests();
	sendToAllClients();
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = container.begin(); it != container.end(); it++)
        close(it->first);
    container.clear();
}

int Server::error(std::string msg) {
	std::cerr << "Server error: " << msg << std::endl;
	return (0);
}


