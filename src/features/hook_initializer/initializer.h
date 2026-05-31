#pragma once

#include <functional>
#include <vector>

class HookInitManager
{
public:
	static HookInitManager& get()
	{
		static HookInitManager instance;
		return instance;
	}

	void add(std::function<void()> initFunc);
	void init_all();

private:
	std::vector<std::function<void()>> m_hooks;
};

#define MARK_FOR_INIT_INTERNAL(func, line) \
__attribute__((constructor)) static void _hook_init_##line() \
{ \
	HookInitManager::get().add(func); \
}

#define MARK_FOR_INIT(func) MARK_FOR_INIT_INTERNAL(func, __COUNTER__)

