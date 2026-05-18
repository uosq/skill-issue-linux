#include "logs.h"

static const char* GetLevelName(LogLevel level)
{
	switch (level)
	{
        case LogLevel::INFO:
		return "INFO - ";
        case LogLevel::WARN:
		return "WARN - ";
        case LogLevel::ERROR:
		return "ERROR - ";
	default:
		return "OH NO EVEN THE LOG'S LEVEL IS FUCKED - ";
        }
}

void Logs::InsertLog(LogLevel level, const std::string &text)
{
	// i love std::string
	// fuck C's strings tho
	m_logs += GetLevelName(level) + text + "\n";
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

const std::string& Logs::GetFullText() const
{
	return m_logs;
}