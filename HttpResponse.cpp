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


// GET запрос
void HttpResponse::get() {
	//находим файл в указанном локейшне
	std::string path = root + reqMap["location"] + fileName;
	int fd = open(path.c_str(), 'r');
	//получаем размер файла
	struct stat statbuf;
	if (fstat(fd, &statbuf) != 0)
		std::cout << "Error opening file" << std::endl;
	else
	{
		char send_buff[statbuf.st_size];
		int nread;
		if ((nread = read(fd, send_buff, statbuf.st_size)) < 0) {
			std::cout << "Error reading" << path << std::endl;
			return;
		}
		net.send(connection, "HTTP/1.1 200 OK\nContent-type: text/html\n\n", 41);
		net.send(connection, send_buff, nread);
		close(fd);
	}

}

HttpResponse::~HttpResponse() {
	close(connection);
}
