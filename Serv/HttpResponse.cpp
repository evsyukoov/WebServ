//
// Created by Денис on 14/12/2020.
//

#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const std::map<std::string, std::string> &reqMap,
		int connection, const std::string &fileName, const std::string &root)
{
	this->reqMap = reqMap;
	this->connection = connection;
	this->fileName = fileName;
	this->root = root;
}

void HttpResponse::manager() {
	if (reqMap["method"] == "GET")
		get();
	else if (reqMap["method"] == "POST")
		return ;
	else if (reqMap["method"] == "HEAD")
	    return ;
}

void HttpResponse::head() {
    //находим файл в указанном локейшне
    std::string path = root + reqMap["location"] + fileName;
    int fd;
    if ((fd = open(path.c_str(), 'r')) < 0)
    {
        std::cout << "Error opening file" << std::endl;
        return ;
    }
    //получаем размер файла
    struct stat statbuf;
    if (fstat(fd, &statbuf) != 0) {
        ::send(connection, "HTTP/1.1 404 Not Found\n\n404 not found", ft_strlen("HTTP/1.1 404 Not Found\n\n404 not found"), 0);
    }
    else
    if ((sendHTML(fd, "HTTP/1.1 405 OK\nContent-type: text/html\n\n\0", statbuf.st_size + 1) < 0))
        return ;
}

//POST

void HttpResponse::post() {
    //находим файл в указанном локейшне
    std::string path = root + reqMap["location"] + fileName;
    int fd;
    if ((fd = open(path.c_str(), 'r')) < 0)
    {
        std::cout << "Error opening file" << std::endl;
        return ;
    }
    //получаем размер файла
    struct stat statbuf;
    if (fstat(fd, &statbuf) != 0) {
        ::send(connection, "HTTP/1.1 404 Not Found\n\n404 not found", ft_strlen("HTTP/1.1 404 Not Found\n\n404 not found"), 0);
    }
    else
        if ((sendHTML(fd, "HTTP/1.1 405 OK\nContent-type: text/html\n\n\0", statbuf.st_size + 1) < 0))
            return ;
}

// GET запрос
void HttpResponse::get() {
	//находим файл в указанном локейшне
	std::string path = root + reqMap["location"] + fileName;
	int fd;
	if ((fd = open(path.c_str(), 'r')) < 0)
	{
		std::cout << "Error opening file" << std::endl;
		return ;
	}
	//получаем размер файла
	struct stat statbuf;
	if (fstat(fd, &statbuf) != 0) {
		::send(connection, "HTTP/1.1 404 Not Found\n\n404 not found", ft_strlen("HTTP/1.1 404 Not Found\n\n404 not found"), 0);
	}
	else
		if ((sendHTML(fd, "HTTP/1.1 200 OK\nContent-type: text/html\n\n\0", statbuf.st_size + 1) < 0))
			return ;
}

HttpResponse::~HttpResponse() {

}

//void HttpResponse::post() {
//	return ;
//}

int HttpResponse::sendHTML(int fd, char *header, int file_size)
{
	char send_buff[file_size];
	std::cout << "file_size: " << file_size << std::endl;
	int nread;
	if ((nread = read(fd, send_buff, file_size)) < 0) {
		std::cout << "Error reading"  << std::endl;
		return (-1);
	}
	send_buff[nread] = '\0';
	std::cout << "Send" << std::endl;
	::send(connection, header, ft_strlen(header), 0);
	::send(connection, send_buff, nread, 0);
	close(fd);
	return (1);
}
