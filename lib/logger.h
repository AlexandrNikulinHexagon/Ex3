#pragma once


#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <thread>

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
	void _run();
	SafeQueue<std::string>& m_queue;
	std::thread m_thread;
};

class ConsoleLoggerHandler: public BaseLoggerHandler
{
public:
	ConsoleLoggerHandler(SafeQueue<std::string>& queue) : BaseLoggerHandler(queue){}

	virtual void process_message(const std::string& msg) override;
};

class FileLoggerHandler : public BaseLoggerHandler
{
	size_t counter = 0;
public:
	FileLoggerHandler( SafeQueue<std::string>& queue) : BaseLoggerHandler(queue) { }

	virtual void process_message(const std::string& msg) override;
};

class Logger
{
	struct LoggerPool
	{		
		SafeQueue<std::string> queue;
		std::list<std::unique_ptr<BaseLoggerHandler>> handlers;
		
		~LoggerPool();
	};
		
public:

	static Logger& get();
	void log(const std::string& msg);

	template<typename T>
	void add_pool(int number)
	{
		auto pool = m_logger_pools.emplace(m_logger_pools.end());
		for (int i = 0; i < number; i++)
			pool->handlers.emplace_back(new T(pool->queue));
	}

private:
	std::list<LoggerPool> m_logger_pools;
};
