#include "hookmanager.hpp"
#include <array>

funchook_t* CHookManager::s_pFuncHook = nullptr;
std::array<CHookManager::HookFn, CHookManager::s_nMaxHooks> CHookManager::s_vecHooks;
int CHookManager::s_nHookCount = 0;

void CHookManager::AddHook(CHookManager::HookFn hook)
{
	if (CHookManager::s_nHookCount >= s_nMaxHooks)
		return;

	CHookManager::s_vecHooks[CHookManager::s_nHookCount] = hook;
	CHookManager::s_nHookCount++;
}

void CHookManager::RemoveHook(HookFn hook)
{
	// todo lmao
}

void CHookManager::LoadAllHooks()
{
	if (CHookManager::s_nHookCount <= 0)
		return;

	CHookManager::s_pFuncHook = funchook_create();

    	if (!CHookManager::s_pFuncHook)
        	return;

	for (int i = 0; i < CHookManager::s_nHookCount; i++)
	{
		auto& fn = CHookManager::s_vecHooks[i];

		if (fn) { fn(); }
	}

	(void)funchook_install(CHookManager::s_pFuncHook, 0);
}

void CHookManager::UnloadHooks()
{
	if (!CHookManager::s_pFuncHook)
		return;

	if (CHookManager::s_nHookCount <= 0)
		return;

	(void)funchook_uninstall(CHookManager::s_pFuncHook, 0);
	(void)funchook_destroy(CHookManager::s_pFuncHook);

	CHookManager::s_pFuncHook = nullptr;
}