//
// Created by Денис on 13/12/2020.
//
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#include <zconf.h>
#include "Net.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "CGI.hpp"


int     handle_connection(int client_sock, Net net)
{
    char recieve[4096];

    std::cout << "Wait for reading request from client" << std::endl;
    usleep(1000);
    int len = net.recv(client_sock, recieve, 4096);
    if (len == -1) {
        std::cout << "Not ready for reading" << std::endl;
        return (-1);
    }
    std::cout << RED << "PARSE MAP: " << RESET << std::endl;
    HttpRequest httpRequest(recieve, len);
    httpRequest.printMap();
    std::cout << GREEN << "HEADER FROM BROWSER: " << RESET << std::endl;
    write(1, recieve, len);
    HttpResponse httpResponse(net, httpRequest.getRequestMap(),
                              client_sock, "index.html",".");
    httpResponse.manager();
    std::cout << BLUE << "Server response html to client" << RESET << std::endl;
    return (1);

}

int main(int argc, char **argv)
{
	char buff[16] = "127.0.0.1:8000";
	Net net(buff);
	int listen = net.listen();
	if (listen < 0)
		return (-1);
	std::cout << "Server is listening..." << std::endl;
    //установим сет дескрипторов current будет отправляться в listen и
    //поскольку listen его модифицирует будем на каждом витке цикла сохранять set до модификации в master_set
    fd_set  current_set;
    fd_set  master_set;
    //максимальное количество сокетов в сетеж
    int max = listen;
    // добавим наше соединение в сет открытых дескрипторов(оно пока одно, сервера)
    FD_SET(listen, &current_set);
    while (true)
	{
        master_set = current_set;
        //select блокирует и ждет пока хотя бы один дескриптор в наборе изменит свое состояние
        std::cout << "Select block..." << std::endl;
        if (select(max + 1, &current_set, NULL, NULL, NULL) == -1)
        {
            std::cout << "Select error" << std::endl;
            return (-1);
        }
        std::cout << "Select unblock.." << std::endl;
        //тут select вернул управление программе
        // пробегаемся по всем дескрипторам в наборе и смотрим какие из них готовы
        for (int i = 0; i <= max; i++)
        {
            if (FD_ISSET(i, &current_set))
            {
                //принимаем соединение
                if (i == listen)
                {
                    int client_sock = net.accept(listen);
//                    const int flags = fcntl(client_sock, F_GETFL, 0);
//                    fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);
                    FD_SET(client_sock, &current_set);
                    if (client_sock > max)
                        max = client_sock;
                    std::cout << "client sock connected: " << client_sock << std::endl;
                }
                //иначе обрабатываем соединение(оно уже есть)
                else
                {
                    handle_connection(i, net);
                    //обработали и удалили из сета
                    FD_CLR(i, &current_set);

                }
            }
        }
	}
	net.close(listen);
	return (1);
}


