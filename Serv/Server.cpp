//
// Created by Casie Carl on 12/18/20.
//

#include "Server.hpp"


int     set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}


HttpRequest     *Server::receiveData(int client_sock)
{
    char recieve[4096];

    std::cout << "Wait for reading request from client" << std::endl;
    int len = read(client_sock, recieve, 4096);
    if (len == 0) {
        //std::cout << "Not ready for reading" << std::endl;
        return (NULL);
    }
    std::cout << "recieved len: " << len << std::endl;
    std::cout << RED << "PARSE MAP: " << RESET << std::endl;
    HttpRequest *httpRequest = new HttpRequest(recieve, len);
    httpRequest->printMap();
    std::cout << GREEN << "HEADER FROM BROWSER: " << RESET << std::endl;
    write(1, recieve, len);
    return (httpRequest);
}

int     Server::sendData(int client_sock, HttpRequest *httpRequest)
{
    HttpResponse httpResponse(httpRequest->getRequestMap(),
                              client_sock, "index.html",".");

    httpResponse.manager();
    std::cout << BLUE << "Server response html to client" << RESET << std::endl;
    shutdown(client_sock, SHUT_RDWR);
    return (1);
}


Server::Server(const std::list<Net> &servers)
{
    this->servers = servers;
    FD_ZERO(&read_set);
}

int Server::openServers()
{
    for(std::list<Net>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        if (it->listen() == -1)
            return (-1);
        it->set_nonblock(it->getListener());

    }
    return (1);
}

void     Server::initSets()
{
    for(std::list<Net>::iterator it = servers.begin(); it != servers.end(); it++) {
        FD_SET(it->getListener(), &read_set);
    }
}

int     Server::run()
{
    if (openServers() == -1)
        return (-1);
    servLoop();
}

int Server::servLoop() {
    //список всех подключенных клиентов
    std::list<int> clients;
    while (true)
    {
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        initSets();
        for(std::list<int>::iterator it = clients.begin(); it != clients.end(); it++) {
            FD_SET(*it, &read_set);
            FD_SET(*it, &write_set);
        }
        //ждем коннекта или готовности к чтению
        int max= std::max(std::max_element(servers.begin(), servers.end())->getListener(),*std::max_element(clients.begin(), clients.end()));

        std::cout << "select block" << std::endl;
        select(max + 1, &read_set, NULL, NULL, NULL);
        std::cout << "select unblock, max: " << max << std::endl;
        //бежим по всем серверам, смотрим на каком событие
        for (std::list<Net>::iterator it = servers.begin(); it != servers.end(); it++)
        {
            // произошел коннект на n-ом сервере
            if (FD_ISSET(it->getListener(), &read_set))
            {

                int client_sock = it->accept(it->getListener());
                set_nonblock(client_sock);
                clients.push_back(client_sock);
                std::cout << "listener =  " << it->getListener() <<  " client_sock = "  << client_sock << std::endl;
            }
        }
        //смотрим что кто то из клиентов что-то отправил(сервер готов читать)
        for (std::list<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if (FD_ISSET(*it, &read_set))
            {
                HttpRequest *req;
                if (!(req = receiveData(*it)))
                {
                    close(*it);
                    clients.erase(it);
                }
                else
                {
                    if (FD_ISSET(*it, &write_set))
                        sendData(*it, req);
                }
                delete req;
            }
        }
    }
}

Server::~Server() {
    for (std::list<Net>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        close(it->getListener());
        servers.erase(it);
    }
    servers.clear();
}


