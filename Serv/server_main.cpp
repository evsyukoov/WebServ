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

int main()
{
	char buff[16] = "127.0.0.1:8000";
	Net net(buff);
	int listen = net.listen();
	if (listen < 0)
		return (-1);
	std::cout << "Server is listening..." << std::endl;

	while (true)
	{
		std::cout << "Wait for client..." << std::endl;
		int accept = net.accept(listen);
		if (accept < 0)
			return (-1);

		char recieve[4096];
		//Приняли
		int len = net.recv(accept, recieve, 4096);
		std::cout << RED << "PARSE MAP: " << RESET << std::endl;
		HttpRequest httpRequest(recieve, len);
		httpRequest.printMap();
		std::cout << GREEN << "HEADER FROM BROWSER: " << RESET << std::endl;
		write(1, recieve, len);
		HttpResponse httpResponse(net, httpRequest.getRequestMap(),
				accept, "index.html",".");
		httpResponse.manager();
		std::cout << BLUE << "Server response html to client" << RESET << std::endl;
	}
	net.close(listen);
	return (1);
}

