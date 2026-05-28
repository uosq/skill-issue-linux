#pragma once

#include <atomic>

class CApp
{
public:
	~CApp() = default;
	CApp();

	bool IsInitialized();
	void Setup();

private:
	bool StartHooks();
	std::atomic<bool> m_bInitialized;
};

extern CApp* gApp;