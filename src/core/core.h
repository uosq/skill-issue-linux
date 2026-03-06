#pragma once

class CApp
{
public:
	CApp();

	CApp& App();
	bool IsInitialized();
	bool StartInterfaces();
	bool StartHooks();
private:
	bool m_bInitialized;
};

extern CApp gApp;