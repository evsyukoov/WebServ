//
// Created by Casie Carl on 12/18/20.
//

#ifndef SERV_SERVER_HPP
#define SERV_SERVER_HPP

#include <map>
//#include <list>
#include <fcntl.h>
#include "Parser/Config.hpp"
#include "HTTP.hpp"
#include "Client.hpp"
#include "AServer.hpp"

//debug
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define WS_BUFFER_SIZE 100000

class Server : public AServer
{
	//сервера ключ-listener, значение -  ServConf
    std::string host;
	std::list<Client *> clients;

	int receiveData(int client_sock, std::string &str);

	int listen(const ServConf &servConf);

	int openServers();


	void iterateWriteSet();
	void acceptConnection(int sockFd);

	void readRequests();
	void sendToAllClients();

	int 	error(std::string msg);

    int    findServerName(const std::string &server_name, Client *client);

    std::pair<std::string, std::string>         &parseHostHeader(const std::string &server_name);

    void doTheJob();

public:
	Server(input &in, const Config &config);

	virtual ~Server();

	int run(HTTP &http);

};

int set_nonblock(int fd);

#endif //SERV_SERVER_HPP
