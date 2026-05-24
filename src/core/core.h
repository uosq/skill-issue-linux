#pragma once

#include <atomic>

class CApp
{
public:
	~CApp() = default;
	CApp();

	bool IsInitialized();
	void Setup();
	bool StartInterfaces();
	bool StartHooks();

private:
	std::atomic<bool> m_bInitialized;
};

extern CApp* gApp;