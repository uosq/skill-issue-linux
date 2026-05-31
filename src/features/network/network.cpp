#include "network.h"

#include <cstring>
#include <curl/curl.h>

#include "../../sdk/interfaces/interfaces.h"

struct memory_struct {
    std::string memory;
};

static size_t mem_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	memory_struct* mem = static_cast<memory_struct*>(userp);

	try
	{
		mem->memory.append(static_cast<const char*>(contents), realsize);
	}
	catch (const std::bad_alloc& e)
	{
		// out of memory
		// we are fucked
		interfaces::Cvar->ConsoleColorPrintf({255, 100, 100, 255}, "Network error: Not enough memory to download file\n");
		return 0; // abort shit
	}

	return realsize;
}

bool CNetwork::init()
{
	initialized = false;
	CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);

	if (code != CURLcode::CURLE_OK)
		return false;

	initialized = true;
	return true;
}

std::string CNetwork::request(const std::string& url, bool& success)
{
	success = false;
	if (!initialized)
		return "";

	// i know making a curl every request is not a good idea
	// but i dont want to mess with multithreading
	CURL* local_curl = curl_easy_init();
	if (!local_curl)
		return "";

	memory_struct chunk;
	char error_buffer[CURL_ERROR_SIZE] = { 0 };

	curl_easy_setopt(local_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(local_curl, CURLOPT_WRITEFUNCTION, mem_callback);
	curl_easy_setopt(local_curl, CURLOPT_WRITEDATA, static_cast<void*>(&chunk));
	curl_easy_setopt(local_curl, CURLOPT_ERRORBUFFER, error_buffer);
	
	curl_easy_setopt(local_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(local_curl, CURLOPT_ACCEPT_ENCODING, ""); // compress to gzip
	curl_easy_setopt(local_curl, CURLOPT_FOLLOWLOCATION, 1L); // allow redirects
	curl_easy_setopt(local_curl, CURLOPT_MAXREDIRS, 5L); // max redirects
	
	// only wait for like 15 seconds
	// maybe reduce this?
	// maybe 5 seconds would be better
	// cant make the game freeze for 15 whole seconds, that would be crazy
	curl_easy_setopt(local_curl, CURLOPT_CONNECTTIMEOUT, 5L);  
	curl_easy_setopt(local_curl, CURLOPT_TIMEOUT, 15L);        
	curl_easy_setopt(local_curl, CURLOPT_SSLVERSION, static_cast<long>(CURL_SSLVERSION_TLSv1_2));

	CURLcode result = curl_easy_perform(local_curl);

	if (result != CURLE_OK)
	{
		interfaces::Cvar->ConsoleColorPrintf({255, 100, 100, 255}, "Network Error: %s\n", error_buffer);
		curl_easy_cleanup(local_curl);
		return "";
	}

	curl_easy_cleanup(local_curl);

	success = true;
	return chunk.memory;
}

void CNetwork::shutdown()
{
	if (initialized)
	{
		curl_global_cleanup();
		initialized = false;
	}
}

bool CNetwork::is_initialized()
{
	return initialized;
}