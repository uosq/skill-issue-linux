#pragma once

class CApp
{
      public:
	CApp();

	bool IsInitialized();
	void Setup();
	bool StartInterfaces();
	bool StartHooks();

      private:
	bool m_bInitialized;
};

extern CApp gApp;