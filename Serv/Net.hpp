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
#include <algorithm>

class Net {
	char 	*address;
	int 	port;
	char 	*net_addr;
	int     listener;
public:
    int getListener() const;

public:
	Net();

	virtual ~Net();

	Net(char *address);

	int 	listen();

	int 	accept(int listen);

	int 	connect();

	int 	close(int fd);

	int 	send(int connection, char *buff, int size);

	int 	recv(int connection, char *buff, int size);

	int 	error(std::string msg);

	void 	parseAddr();

    //функция делающая сокеты для read и write неблокирующими

    int     set_nonblock(int fd);

    friend bool operator < (const Net &n1, const Net &n2) ;

    friend bool operator > (const Net &n1, const Net &n2) ;
};

#endif
