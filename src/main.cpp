#include "core/core.h"

#include <cstdio>
#include <thread>
#include <chrono>

#include "sdk/interfaces/interfaces.h"

static void InitThread()
{
	while (true)
	{
		if (init_factories())
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	while (true)
	{
		if (init_interfaces() && init_global_interfaces())
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	gApp->Setup();

	std::fprintf(stdout, "[+] Skill issue successfully attached and initialized!\n");
}

__attribute__((constructor)) void init(void)
{
	// never trust C++ initialization
	// fuck C++
	gApp = new CApp();

	std::thread init_thread(InitThread);
	init_thread.detach();
}

__attribute__((destructor)) void unload(void)
{
	if (gApp)
	{
		delete gApp;
		gApp = nullptr;
	}
}