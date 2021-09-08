#pragma once


#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>

#include "safequeue.h"

class BaseLoggerHandler
{
public:
	class Stop : public std::exception { };

	BaseLoggerHandler(SafeQueue<std::string>& queue) : m_queue(queue), m_thread(&BaseLoggerHandler::_run, this)	{ }
	virtual ~BaseLoggerHandler()
	{
		m_thread.join();
	}

protected:
	virtual void process_message(const std::string& msg) = 0;
	void _run()
	{
		try
		{
			while (true)
				process_message(m_queue.pop());
		}
		catch (const SafeQueue<std::string>::Stop&)
		{
		}
		catch (...)
		{
		}
	};
	SafeQueue<std::string>& m_queue;
	std::thread m_thread;
};

class ConsoleLoggerHandler: public BaseLoggerHandler
{
public:
	ConsoleLoggerHandler(SafeQueue<std::string>& queue) : BaseLoggerHandler(queue){}

	virtual void process_message(const std::string& msg) override
	{
		std::cout << "bulk: " << msg << std::endl;
	}
};

class FileLoggerHandler : public BaseLoggerHandler
{
	size_t counter = 0;
public:
	FileLoggerHandler( SafeQueue<std::string>& queue) : BaseLoggerHandler(queue) { }

	virtual void process_message(const std::string& msg) override
	{		
		std::stringstream ss;
		ss << "bulk" << std::time(nullptr) << "_" << std::this_thread::get_id() << "_" << ++counter << ".log";
		std::ofstream f(ss.str(), std::ios_base::ate | std::ios_base::app);
		if (f.tellp() != 0)
			f << std::endl;

		f << msg;
	}
};

class Logger
{
	struct LoggerPool
	{		
		SafeQueue<std::string> queue;
		std::list<std::unique_ptr<BaseLoggerHandler>> handlers;
		
		~LoggerPool()
		{
			queue.wait_empty();
		}
	};
		
public:
	static Logger& get()
	{
		static Logger logger;
		return logger;
	}

	void log(const std::string& msg)
	{
		for(auto& pool: m_logger_pools)
			pool.queue.push(msg);
	}
	
	template<typename T>
	static void add_pool(int number)
	{
		auto pool = m_logger_pools.emplace(m_logger_pools.end());
		for (int i = 0; i < number; i++)
			pool->handlers.emplace_back(new T(pool->queue));
	}
private:
	static inline std::list<LoggerPool> m_logger_pools;
};
