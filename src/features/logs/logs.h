#pragma once

#include <string>
#include <vector>

#include "../feature.h"

enum class LogLevel
{
	INFO = 0,
	WARN,
	ERROR
};

class Logs
{
public:
	void Info(const std::string &text);
	void Warn(const std::string &text);
	void Error(const std::string &text);

	void Clear();

	const std::string& GetFullText() const;
private:
	void InsertLog(LogLevel level, const std::string& text);

	std::string m_logs;
};

DECLARE_FEATURE(Logs, logs)