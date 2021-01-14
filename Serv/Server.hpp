//
// Created by Casie Carl on 12/18/20.
//

#ifndef SERV_SERVER_HPP
#define SERV_SERVER_HPP
#include <map>
#include <list>
#include <fcntl.h>
#include "Parser/Config.hpp"
#include "HTTP.hpp"
#include "Client.hpp"

//debug
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define WS_BUFFER_SIZE 100000

class Server {
    //сервера ключ-listener, значение -  ServConf
    std::map<int, ServConf>              servers;


    Config 						config;
    //сет всех дескрипторов для select
    fd_set                      read_set;
    fd_set                      write_set;
    int                         max;
    struct input in;

    int   receiveData(int client_sock, std::string &str);

	int listen(const ServConf &servConf);

public:
   Server(input &in, const Config &config);

    virtual ~Server();

    int    openServers();

    int     servLoop(HTTP &http);


    int     run(HTTP &http);

    void    initReadSet();
    void    resetFdSets();

    std::vector<char*>      readRequests(std::list<Client*> &clients);

    void	sendToAllClients(std::vector<char*> requests, std::list<Client*> &clients, HTTP &http);

};

int     set_nonblock(int fd);

#endif //SERV_SERVER_HPP
