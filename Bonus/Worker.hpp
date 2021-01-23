//
// Created by Filipp Ewseew on 1/23/21.
//

#ifndef WORKER_HPP
# define WORKER_HPP
# include "AServer.hpp"
# include <queue>
# define WS_BUFFER_SIZE 100000

static std::queue<Client *> g_queue;
static pthread_mutex_t      g_qLock;

class Worker : public AServer
{
	pthread_mutex_t queueLock;
	HTTP    http;

	void    iterateReadSet();

	void    iterateWriteSet();

	int     receiveData(int client_sock, std::string &str);

	void    readRequests();

	void    sendToAllClients();

	void    doTheJob();

	int     findServerName(const std::string &server_name, Client *client);

 public:
	Worker(input inp, Config const &config);

	~Worker();

	Client *getTask()
	{
		Client *ret;

		pthread_mutex_lock(&g_qLock);
		if (g_queue.empty())
			ret = nullptr;
		else
		{
			ret = g_queue.front();
			g_queue.pop();
		}
		pthread_mutex_unlock(&g_qLock);
		return ret;
	}

	void    runWorker();

	void    addClient(std::pair<int, Client *> client);
};

static inline void *workerLoop(void *arg)
{
	Worker *w = reinterpret_cast<Worker *>(arg);
	Client *c;
	std::pair<int, Client *> conn;

	PRINT("Worker live.")
	while (true)
	{
		if ((c = w->getTask()))
		{
			conn.first = c->getClientSock();
			conn.second = c;
			w->addClient(conn);
		}
		w->servLoop();
	}
}

#endif