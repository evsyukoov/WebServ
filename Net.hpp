//
// Created by Денис on 13/12/2020.
//

#ifndef SERV_NET_HPP
#define SERV_NET_HPP
#include <string>
#include "sys/types.h"
#include <arpa/inet.h>
#include <iostream>
#include "lib.hpp"
#include <unistd.h>

class Net {
	char 	*address;
	int 	port;
	char 	*net_addr;

public:
	Net(char *address);

	int 	listen();

	int 	accept(int listen);

	int 	connect();

	int 	close(int fd);

	int 	send(int connection, char *buff, int size);

	int 	recv(int connection, char *buff, int size);

	int 	error(std::string msg);

	void 	parseAddr();
};

#endif
