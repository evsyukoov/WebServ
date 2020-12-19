//
// Created by Casie Carl on 12/18/20.
//

#ifndef SERV_SERVER_HPP
#define SERV_SERVER_HPP
#include <map>
#include <list>
#include <fcntl.h>
#include "Net.hpp"
#include "HttpRequest.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

//debug
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

class Server {
    //количество серверов( localhost::80, localhost::443 и.т.д
    std::list<Net>              servers;
    //сет всех дескрипторов для select
    fd_set                      read_set;
    fd_set                      write_set;

    HttpRequest     *receiveData(int client_sock);

    int    sendData(int client_sock, HttpRequest *httpRequest);

public:
    Server(const std::list<Net> &servers);

    virtual ~Server();

    int    openServers();

    int     servLoop();

    int     handle_connection(int client_sock);

    int     run();

    void    initReadSet();

	std::vector<HttpRequest*>	readRequests(std::list<int> &clients);

	void 			sendToAllClients(std::vector<HttpRequest*> requests, std::list<int> clients);

};

int     set_nonblock(int fd);

#endif //SERV_SERVER_HPP
