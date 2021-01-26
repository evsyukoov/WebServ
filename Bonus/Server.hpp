//
// Created by Casie Carl on 12/18/20.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Threadpool.hpp"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

class Server : public AServer, public Threadpool<Worker, Client *>
{
 private:
	std::vector<Worker> proletariat;
	std::string host;

	int listen(const ServConf &servConf);

	int openServers();

	void resetFdSets();

	void iterateWriteSet() {};

	void acceptConnection(int sockFd);

	int error(std::string msg);


 public:
	Server(input &in, const Config &config);

	virtual ~Server();

	void doTheJob();

	int run();

};

int set_nonblock(int fd);

#endif //SERV_SERVER_HPP

