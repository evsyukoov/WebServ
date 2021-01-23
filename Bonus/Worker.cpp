//
// Created by Filipp Ewseew on 1/23/21.
//

#include "Worker.hpp"

void    Worker::iterateReadSet()
{
	pthread_mutex_lock(&queueLock);
	AServer::iterateReadSet();
	pthread_mutex_unlock(&queueLock);
}

void    Worker::iterateWriteSet()
{
	std::map<int, Client*>::iterator iter;
	Client *client;

	pthread_mutex_lock(&queueLock);
	for (iter = container.begin(); iter != container.end(); ++iter)
	{
		client = iter->second;
		if (client->getState() == WRITING || client->getState() == WRITING_BODY)
			FD_SET(iter->first, &write_set);
		max = std::max(max, iter->first);
	}
	pthread_mutex_unlock(&queueLock);
}

int   Worker::receiveData(int client_sock, std::string &str)
{
	char recieve[WS_BUFFER_SIZE + 1];

	int len = read(client_sock, recieve, WS_BUFFER_SIZE);
	if (len <= 0)
		return (len);
	recieve[len] = '\0';
	str = recieve;
	//std::cout << BLUE << "recieve: " << str  << RESET << std::endl;
	return (1);
}

void    Worker::readRequests()
{
	std::vector<char*> requests;
	std::string req;
	std::string data;
	Client *client;

	std::map<int, Client*>::iterator it = container.begin();
	std::map<int, Client*>::iterator ite = container.end();
	while (it != ite)
	{
		if (FD_ISSET(it->first, &read_set))
		{
			client = it->second;
			int ret = receiveData(it->first, data);

			if (ret == 0)
			{
				close(it->first);
				delete (client);
				it = container.erase(it);
				continue ;
			}
			else if (ret > 0)
				client->findState(data);
		}
		++it;
	}
}

static int sendBodySegment(Response *rs)
{
	StringResponse *sr = dynamic_cast<StringResponse *>(rs);

	if (sr != nullptr)
		return (sr->sendPiece());
	return (dynamic_cast<FileResponse *>(rs)->sendPiece());
}


int    Worker::findServerName(const std::string &server_name, Client *client)
{
	size_t pos = server_name.find(':');
	std::string host_header = server_name.substr(0, pos);
	std::string port_header = server_name.substr(pos + 1);
	int port = std::stoi(port_header);
	std::list<ServConf> serv = config.getConfig();
	//ищем совпадение на server_name
	for(std::list<ServConf>::iterator it = serv.begin(); it != serv.end(); it++)
	{
		if (port == it->getPort() && host_header == it->getServerName()) {
			client->setServConf(*it);
			return (1);
		}
	}
	//если не нашли
	if (host_header != "localhost" && host_header != "127.0.0.1")
		return (0);
	//иначе ищем первое совпадение по порту
	for(std::list<ServConf>::iterator it = serv.begin(); it != serv.end(); it++)
	{
		if (port == it->getPort())
		{
			client->setServConf(*it);
			return (1);
		}
	}
	return (1);
}

void	Worker::sendToAllClients()
{
	std::string data;
	Client *client;

	std::map<int, Client*>::iterator it = container.begin();
	std::map<int, Client*>::iterator ite = container.end();
	Response *r;

	while (it != ite)
	{
		if (FD_ISSET(it->first, &write_set))
		{
			client = it->second;
			if (client->getState() == WRITING)
			{
				in.remote_addr = client->getRemoteAddr();
				std::map<std::string, std::string> map(client->getReqMap());
				HTTP http;

				//отдаем пустую мапу, если не нашли host, после обработки получим 400
				if (map.count("Host") == 0 || !(findServerName(map["Host"], client)))
					client->clearRequest();
				//printReqMap(map);
				http.setFields(client->getClientSock(), client->getBody(),
						client->getServConf(), in, client->getReqMap());
				http.manager();
				r = http.getResponse();
				client->setResponse(r);
				client->getResponse()->sendHeader();
			}
			else if (client->getState() == WRITING_BODY)
			{
				if (!sendBodySegment(client->getResponse()))
					client->clear();
			}

		}
		it++;
	}
}

void Worker::doTheJob()
{
	readRequests();
	sendToAllClients();
}

void Worker::addClient(std::pair<int, Client *> client)
{
	pthread_mutex_lock(&queueLock);
	container.insert(client);
	pthread_mutex_unlock(&queueLock);
}

Worker::Worker(input inp, Config const &config) : AServer(inp, config)
{
	pthread_mutex_init(&queueLock, nullptr);
};

Worker::~Worker()
{
	pthread_mutex_destroy(&queueLock);
};