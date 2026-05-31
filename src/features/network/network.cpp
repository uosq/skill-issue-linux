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

__always_inline static void set_curl_timeout(CURL* curl, int connect_timeout = 5, int timeout = 15)
{
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, static_cast<long>(connect_timeout));
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, static_cast<long>(timeout));
	curl_easy_setopt(curl, CURLOPT_SSLVERSION, static_cast<long>(CURL_SSLVERSION_TLSv1_2));
}

__always_inline static void set_curl_redirects(CURL* curl, int follow_location = 1, int max_redirs = 5)
{
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, static_cast<long>(follow_location)); // allow redirects
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, static_cast<long>(max_redirs)); // max redirects
}

__always_inline static void enable_curl_compresson(CURL* curl)
{
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, ""); // compress to gzip
}

__always_inline static void set_curl_useragent(CURL* curl, const char* user_agent = "libcurl-agent/1.0")
{
	curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
}

__always_inline static void set_curl_write_callback(CURL* curl, void* cb)
{
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
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
	curl_easy_setopt(local_curl, CURLOPT_WRITEDATA, static_cast<void*>(&chunk));
	curl_easy_setopt(local_curl, CURLOPT_ERRORBUFFER, error_buffer);

	enable_curl_compresson(local_curl);

	set_curl_write_callback(local_curl, (void*)&mem_callback);
	set_curl_useragent(local_curl);
	set_curl_redirects(local_curl);
	set_curl_timeout(local_curl);

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