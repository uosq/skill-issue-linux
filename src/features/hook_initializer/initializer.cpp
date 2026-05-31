#include "initializer.h"

void HookInitManager::add(std::function<void()> initFunc)
{
	m_hooks.push_back(initFunc);
}

void HookInitManager::init_all()
{
	for (const auto& func : m_hooks)
		func();
}