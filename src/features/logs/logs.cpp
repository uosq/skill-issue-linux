#include "logs.h"

std::vector<LogEntry> Logs::m_logs = {};

void InsertLog(LogLevel level, const std::string &text)
{
	LogEntry entry;
	entry.level = level;
	entry.text  = text;
	Logs::m_logs.emplace_back(entry);
}

void Logs::Info(const std::string &text)
{
	InsertLog(LogLevel::INFO, text);
}

void Logs::Warn(const std::string &text)
{
	InsertLog(LogLevel::WARN, text);
}

void Logs::Error(const std::string &text)
{
	InsertLog(LogLevel::ERROR, text);
}

const std::vector<LogEntry> &Logs::GetLogs()
{
	return m_logs;
}