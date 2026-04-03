#pragma once

#include <string>
#include <vector>

enum class LogLevel
{
	INFO = 0,
	WARN,
	ERROR
};

struct LogEntry
{
	std::string text;
	LogLevel level;
};

namespace Logs
{
	extern std::vector<LogEntry> m_logs;

	void Info(const std::string &text);
	void Warn(const std::string &text);
	void Error(const std::string &text);

	const std::vector<LogEntry> &GetLogs();
} // namespace Logs