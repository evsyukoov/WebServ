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


int   Server::receiveData(int client_sock, std::string &str)
{
    char recieve[BUFFER_SIZE];

#ifdef D_READ
    std::cout << "Wait for reading request from client: " << client_sock << std::endl;
#endif
    usleep(1000);
    int len;
    len = read(client_sock, recieve, BUFFER_SIZE - 1);
    recieve[len] = '\0';
#ifdef D_READ
    std::cout << "len: " << len << std::endl;
#endif
    if (len <= 0)
        return (len);
    else
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
    for(std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        FD_SET((*it).first, &read_set);
    }
}

int     Server::run(HTTP &http)
{
    if (openServers() == -1)
        return (-1);
    servLoop(http);
    return (1);
}

bool    Comparator(const Client *c1, const Client *c2)
{
    return (c1->getClientSock() < c2->getClientSock());
}

//отладочная печать клиентов

void        print(const std::list<Client*> &clients)
{
    std::cout << RED << "AVAILABLE CLIENTS: ";
    for(std::list<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
        std::cout << (*it)->getClientSock() << " " ;
    std::cout << RESET << std::endl;
}

int Server::servLoop(HTTP &http) {
    //ключ-сокет клиента  - value - конфиг сервера на котором коннект
	std::list<Client*> clients;
	while (true) {
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		initReadSet();
		for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			FD_SET((*it)->getClientSock(), &read_set);

		int max;
		if (!clients.empty())
		    max = std::max((--servers.end())->first, (*std::max_element(clients.begin(), clients.end(), Comparator))->getClientSock());
		else
		    max = (--servers.end())->first;
        //ждем коннекта или готовности к чтению
#ifdef D_SELECT
		std::cout << "select block" << std::endl;
#endif
		int ret = select(max + 1, &read_set,  &write_set, NULL, NULL);
		if (ret < 0) {
            std::cerr << "select error: " << ret << std::endl;
            continue;
        }
#ifdef D_SELECT
		std::cout << "select unblock, ret: " << ret << std::endl;
#endif
		//бежим по всем серверам, смотрим на каком событие
		for (std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++) {
			// произошел коннект на n-ом сервере
			if (FD_ISSET((*it).first, &read_set)) {
				int client_sock = accept((*it).first, NULL, NULL);
				set_nonblock(client_sock);
                Client *client = new Client(client_sock, it->second);
                clients.push_back(client);
#ifdef D_SELECT
				std::cout << "Accept done: " << client_sock << std::endl;
#endif
			}
		}
        std::vector<char*> requests = readRequests(clients);
#ifdef D_SELECT
        print(clients);
#endif
		sendToAllClients(requests, clients, http);
	}
	return (1);
}

std::vector<char*>      Server::readRequests(std::list<Client*> &clients)
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
		    //произошла ошибка, попробуем позднее
            if (ret < 0)
               it++;
			//кто-то отключился
			else if (ret == 0)
			{
				close((*it)->getClientSock());
				delete (*it);
				it = clients.erase(it);
			}
			//если не отключился значит готов принять ответ
			//смотрим что запрос полный
			else if (ret > 0)
			{
                (*it)->findState(data);
                if ((*it)->getState() == FINISH)
                    FD_SET((*it)->getClientSock(), &write_set);
                it++;
#ifdef D_STATE
				if (it != ite)
                 std::cout << "State: " << (*it)->getState() << std::endl;
#endif
			}
			//иначе просто идем дальше
			else
			    it++;
		}
		else
		    it++;
	}
	return (requests);
}

void	Server::sendToAllClients(std::vector<char*> requests, std::list<Client*> &clients, HTTP &http)
{
	int i = 0;
    std::list<Client*>::iterator it = clients.begin();
    std::list<Client*>::iterator ite = clients.end();
	while (it != ite)
    {
		if (FD_ISSET((*it)->getClientSock(), &write_set)) {
            http.setFields((*it)->getClientSock(), (char *) (*it)->getRequest().c_str(), (*it)->getServConf(), in);
            http.manager();
            (*it)->clear();
        }
		it++;
	}

}

Server::~Server() {
    for (std::map<int, ServConf>::iterator it = servers.begin(); it != servers.end(); it++)
        close(it->first);
    servers.clear();
}


