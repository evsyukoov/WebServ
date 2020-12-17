//
// Created by Денис on 13/12/2020.
//
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#include <zconf.h>
#include <list>
#include "Net.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "CGI.hpp"


int     set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

int     handle_connection(int client_sock, Net net)
{
    char recieve[4096];

    std::cout << "Wait for reading request from client" << std::endl;
    int len = net.recv(client_sock, recieve, 4096);
    if (len == 0 && errno != EAGAIN) {
        //std::cout << "Not ready for reading" << std::endl;
        return (-1);
    }
    std::cout << "recieved len: " << len << std::endl;
    //std::cout << RED << "PARSE MAP: " << RESET << std::endl;
    HttpRequest httpRequest(recieve, len);
    httpRequest.printMap();
    //std::cout << GREEN << "HEADER FROM BROWSER: " << RESET << std::endl;
    write(1, recieve, len);
    HttpResponse httpResponse(net, httpRequest.getRequestMap(),
                              client_sock, "index.html",".");

    httpResponse.manager();
    std::cout << BLUE << "Server response html to client" << RESET << std::endl;
    //close(client_sock);
    shutdown(client_sock, SHUT_WR);
    return (1);

}

void    print_clients(std::list<int> lst)
{
    for(auto it = lst.begin(); it != lst.end(); it++)
        std::cout << "socket: " << *it << std::endl;
}

int main(int argc, char **argv)
{
	char buff[16] = "127.0.0.1:8000";
	Net net(buff);
	int listen = net.listen();
	set_nonblock(listen);
	if (listen < 0)
		return (-1);
	std::cout << "Server is listening..." << std::endl;
	std::list<int> clients;
    //максимальное количество сокетов в сети
    // добавим наше соединение в сет открытых дескрипторов(оно пока одно, сервера)
    while (true)
	{
        fd_set  current_set;
        FD_ZERO(&current_set);
        FD_SET(listen, &current_set);
        //добавляем дескрипторы из clients в set для select()
        std::list<int>::iterator it = clients.begin();
        std::list<int>::iterator ite = clients.end();

        for( std::list<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &current_set);
        print_clients(clients);
        int max = *std::max_element(clients.begin(), clients.end());
        if (listen > max)
            max = listen;
        //select блокирует и ждет пока хотя бы один дескриптор в наборе изменит свое состояние
        std::cout << "Select block... max = " << max << std::endl;
        if (select(max + 1, &current_set, NULL, NULL, NULL) == -1)
        {
            std::cout << "Select error" << std::endl;
            return (-1);
        }
        std::cout << "Select unblock.." << std::endl;
        //тут select вернул управление программе
        //смотрим на мастер-сокет, пытался ли кто-нибудь к нему подключиться
        if (FD_ISSET(listen, &current_set))
        {
            int client_sock = net.accept(listen);
            set_nonblock(client_sock);
            clients.push_back(client_sock);
            std::cout << "client sock connected: " << client_sock << std::endl;
        }
        // пробегаемся по всем дескрипторам в наборе и смотрим какие из них готовы
        print_clients(clients);
        std::cout << std::endl;
        for (std::list<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if (FD_ISSET(*it, &current_set))
            {
                std::cout << "socket " << *it << " is ready for read" << std::endl;
                //обрабатываем
              //  handle_connection(*it, net);
                 if (handle_connection(*it, net) == -1)
                 {
                     close(*it);
                     it = clients.erase(it);
                     continue;
                 }
            }
        }
        print_clients(clients);
        std::cout << std::endl;
	}
	net.close(listen);
	return (1);
}


