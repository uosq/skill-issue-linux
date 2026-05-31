#pragma once

#include <curl/curl.h>
#include <string>

#include "../feature.h"

class CNetwork
{
public:
	bool init();
	void shutdown();
	bool is_initialized();

	std::string request(const std::string& url, bool& success);

private:
	bool initialized {false};
};

DECLARE_FEATURE(CNetwork, network)