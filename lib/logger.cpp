#pragma once


#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <thread>

#include "safequeue.h"
#include "logger.h"

void BaseLoggerHandler::_run()
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
}

void ConsoleLoggerHandler::process_message(const std::string& msg)
{
	std::cout << "bulk: " << msg << std::endl;
}

void FileLoggerHandler::process_message(const std::string& msg)
{		
	std::stringstream ss;
	ss << "bulk" << std::time(nullptr) << "_" << std::this_thread::get_id() << "_" << ++counter << ".log";
	std::ofstream f(ss.str(), std::ios_base::ate | std::ios_base::app);
	if (f.tellp() != 0)
		f << std::endl;

	f << msg;
}

Logger& Logger::get()
{
	static Logger logger;
	return logger;
}

void Logger::log(const std::string& msg)
{
	for(auto& pool: m_logger_pools)
		pool.queue.push(msg);
}

Logger::LoggerPool::~LoggerPool()
{
	if(!handlers.empty())
		queue.wait_empty();
}
