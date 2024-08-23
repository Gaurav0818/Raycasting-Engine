#include "Logger.h"
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <sstream>


std::vector<LogEntry> Logger::logEntries;

const std::string GetTimeString() {
	time_t now = time(0);
	tm timeinfo;
	localtime_s(&timeinfo, &now);

	std::ostringstream timestream;
	timestream << std::put_time(&timeinfo, "%Y/%m/%d %H:%M:%S");
	return timestream.str();
}

void Logger::Info(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::INFO;
	entry.message = "INFO :[" + GetTimeString() + "] || " + message;
	logEntries.push_back(entry);


	// Display the message
	std::cout << entry.message << std::endl;
}

void Logger::Warning(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::WARNING;
	entry.message = "WARN :[" + GetTimeString() + "] || " + message;
	logEntries.push_back(entry);


	// Display the message
	std::cout <<"\033[33m"<< entry.message << "\033[0m" << std::endl;
}

void Logger::Error(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::ERROR;
	entry.message = "ERRR :[" + GetTimeString() + "] || " + message;
	logEntries.push_back(entry);


	// Display the message
	std::cerr <<"\033[31m"<< entry.message << "\033[0m" << std::endl;
}
