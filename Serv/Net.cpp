//
// Created by Денис on 13/12/2020.
//

#include <fcntl.h>
#include "Net.hpp"

//address = 172.0.0.1:8000

Net::Net(char *address) {
	this->address = address;
	parseAddr();
}

void Net::parseAddr() {
	char **tmp = ft_split(address, ':');
	net_addr = tmp[0];
	port = htons(ft_atoi(tmp[1]));
}

//для сервера функция которая слушает все входящие соединения на заданном IP и порту
int Net::listen() {
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
		return (error("sock error"));
	int optval = 1;
	//если сокет уже был открыт
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		return error("setsockopt error");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(net_addr);

	if (::bind(listener, ((struct sockaddr*)&addr), sizeof(addr)) != 0)
		return (error("bind failed"));
	if (::listen(listener, SOMAXCONN) != 0)
		return (error("listen error"));
	return (1);

}

int Net::error(std::string msg) {
	std::cerr << msg << std::endl;
	return (-1);
}

//Принимаем соединение от клиента
int Net::accept(int listen) {
	int fd = ::accept(listen, NULL, NULL);
	if (fd < 0)
		return (error("Accept error"));
	return (fd);
}

int Net::close(int fd) {
	return ::close(fd);
}

int Net::send(int connection, char *buff, int size) {
	return (::send(connection, buff, size, 0));
}

int Net::recv(int connection, char *buff, int size) {
	return (::recv(connection, buff, size, 0));
}

//клиент стучится к серверу
int Net::connect() {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
		return (error("sock error"));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(net_addr);
	if (::connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		return (error("connection failed"));
	return (sock_fd);
}

Net::Net() {}

Net::~Net() {

}

int Net::getListener() const {
    return listener;
}

int     Net::set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

bool operator < (const Net &n1, const Net &n2) {
    return n1.listener < n2.listener;
}

bool operator > (const Net &n1, const Net &n2) {
    return n1.listener > n2.listener;
}


