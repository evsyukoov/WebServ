
#ifndef GIT_SERV_HTTP_HPP
#define GIT_SERV_HTTP_HPP
#define YELLOW  "\033[1;33m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include "Parser/Config.hpp"
#include <unistd.h>
#include <limits>
#include <sys/socket.h>

class HTTP
{
private:

	ServConf		servConf;
	std::string 	buff_req;
	std::map<std::string, std::string>	reqMap;  //map запроса включает в себя все заголовки
	std::list<Location>::const_iterator it;
	std::string 	result;
	int				client_fd;

	void 	get();

	void 	post();

	bool checkForAllowedMethod();

//	const ServConf& getServerNum(int num);

	bool locationMatch(const std::string& location);

	std::string pathFormerer();

	int checkDirectory(const std::string& root);

	std::string rootSwitcher(const std::string& root, const std::string& serv_index, const std::string& loc_index);

	int sendReq(std::string header, std::string request);

	void readFile(int file_size, int fd);

	std::list<Location>::const_iterator getMatchingLocation();

	long contentLength();

	void 	initMap();

public:

	HTTP(int client, char *buf, const ServConf&);

	HTTP(); // дефолтный конструктор, не инициализирует ничего

	void setFields(int client, char *buf, const ServConf &serv); // функция инициализации полей для дальнейшей обработки

	std::string &getResponce();

	const std::map<std::string, std::string> &getRequestMap() const; // для отладки

	void 	printMap();  //для отладки

	void 	manager();

};


#endif //GIT_SERV_HTTP_HPP