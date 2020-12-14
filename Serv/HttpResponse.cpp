//
// Created by Денис on 14/12/2020.
//

#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const Net &net, const std::map<std::string, std::string> &reqMap,
		int connection, const std::string &fileName, const std::string &root)
{
	this->reqMap = reqMap;
	this->connection = connection;
	this->fileName = fileName;
	this->root = root;
	this->net = net;
}

void HttpResponse::manager() {
	if (reqMap["method"] == "GET")
		get();
	else if (reqMap["method"] == "POST")
		post();
}

// GET запрос
void HttpResponse::get() {
	//находим файл в указанном локейшне
	std::string path = root + reqMap["location"] + fileName;
	int fd = open(path.c_str(), 'r');
	//получаем размер файла
	struct stat statbuf;
	if (fstat(fd, &statbuf) != 0) {
		net.send(connection, "HTTP/1.1 404 Not Found\n\n404 not found", ft_strlen("HTTP/1.1 404 Not Found\n\n404 not found"));
	}
	else
		if ((sendHTML(fd, "HTTP/1.1 200 OK\nContent-type: text/html\n\n", statbuf.st_size) < 0))
			return ;
}

HttpResponse::~HttpResponse() {
	close(connection);
}

void HttpResponse::post() {
	return ;
}

int HttpResponse::sendHTML(int fd, char *header, int file_size)
{
	char send_buff[file_size];
	int nread;
	if ((nread = read(fd, send_buff, file_size)) < 0) {
		std::cout << "Error reading"  << std::endl;
		return (-1);
	}
	net.send(connection, header, ft_strlen(header));
	net.send(connection, send_buff, nread);
	close(fd);
	return (1);
}
