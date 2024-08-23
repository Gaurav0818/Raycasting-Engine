#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType
{
	INFO,
	WARNING,
	ERROR
};

struct LogEntry
{
	LogType type;
	std::string message;
};

class Logger
{
public:
	static std::vector<LogEntry> logEntries;

	static void Info(const std::string& message);
	static void Warning(const std::string& message);
	static void Error(const std::string& message);
};

#endif

