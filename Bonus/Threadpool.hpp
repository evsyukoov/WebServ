#ifndef THREADPOOL_HPP
# define THREADPOOL_HPP
# include <pthread.h>
# include <unistd.h>
# include <iostream>
# include <vector>
# include <queue>
# include "Worker.hpp"

/* T is the class that performs jobs */

template <typename W, typename T>
class Threadpool
{

	/* Arbitrary values, don't set them too high. */

# define MAX_WORK 32
# define MAX_Q 8192
# define VALID_PARAMS(x, y) (x > 0 && y > 0 && x <= MAX_WORK && y <= MAX_Q)

	enum e_pool_status
	{
		pool_ok    = 0,
		pool_pause = (1 << 0),
		pool_stop  = (1 << 1),
		pool_kill  = (1 << 2)
	};

 protected:
	std::vector<pthread_t> threads;
	std::vector<W>         workers;
	size_t                 qSize;
	std::queue<T>          *q;
	int                    status;

	int const &state() const
	{ return status; }

	class pool_strerror : public std::exception
	{
	 public:
		char const *what() const throw()
		{
			std::string error(strerror(errno));

			error += " errno: " + std::to_string(errno);
			return (error.c_str());
		}
	};

	/* Cannot copy or assign a Threadpool */

	Threadpool &operator=(const Threadpool &copy)
	{
		(void) copy;
		return (*this);
	};

	/* pthread wrappers */

	void lock()
	{
		if (pthread_mutex_lock(&g_qLock))
			throw pool_strerror();
	}

	void unlock()
	{
		if (pthread_mutex_unlock(&g_qLock))
			throw pool_strerror();
	}

	void createThreads()
	{
		pthread_t newThread;

		status = pool_pause;
		for (size_t i = 0; i < workers.size(); ++i)
		{
			if (pthread_create(&newThread, nullptr, &workerLoop, &workers[i]) < 0)
				throw pool_strerror();
			threads.push_back(newThread);
		}
	}

	void gracefulShutdown()
	{
		status        = pool_stop;

		for (size_t i = 0; i < threads.size(); ++i)
		{
			if (pthread_join(threads[i], nullptr) < 0)
				throw pool_strerror();
		}
	}


 public:

	/* Constructors */

	Threadpool() : qSize(0), q(&g_queue), status(0)
	{};

	Threadpool(int queue_size)
	:  qSize(queue_size), q(&g_queue), status(pool_pause)
	{
	}

	void setWorkers(std::vector<W> &_workers)
	{
		workers = _workers;
	};

	/* Destructor */

	virtual ~Threadpool()
	{
		gracefulShutdown();

		if (pthread_mutex_destroy(&g_qLock))
			throw pool_strerror();
	}

	/* Member functions */

	bool addTask(T job)
	{
		bool ret;

		lock();
		if (q->size() == qSize)
			ret = false;
		else
		{
			q->push(job);
			ret = true;
		}
		unlock();
		return ret;
	}

	T getTask()
	{
		T ret;

		lock();
		if (q->empty() || status != pool_ok)
			ret = nullptr;
		else
		{
			ret = q->front();
			q->pop();
		}
		unlock();
		return ret;
	}


	int initThreadpool()
	{
		if (VALID_PARAMS(workers.size(), qSize) == false)
			return (1);

		if (pthread_mutex_init(&g_qLock, nullptr) < 0)
			throw pool_strerror();

		createThreads();
		return (0);
	}
	/* "Signals" */

	void kill()
	{ status = pool_kill; }

	void pause()
	{ status = pool_pause; }

	void startPool()
	{ status = pool_ok; }
};

#endif
