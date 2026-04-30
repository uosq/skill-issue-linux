#pragma once

#include <cstdint>

inline uintptr_t RelToAbs(uintptr_t instr, int dispOffset = 3, int instrSize = 7)
{
	int32_t disp = *reinterpret_cast<int32_t*>(instr + dispOffset);
	return instr + instrSize + disp;
}

template <std::size_t I, typename T, typename... Args> inline T vtable_call(void *p, Args... args)
{
	auto vTable = *static_cast<void ***>(p);
	return reinterpret_cast<T (*)(void *, Args...)>(vTable[I])(p, args...);
}

template <std::size_t I, typename T, typename... Args> inline T vtable_call(uintptr_t u, Args... args)
{
	auto p	    = reinterpret_cast<void *>(u);
	auto vTable = *static_cast<void ***>(p);
	return reinterpret_cast<T (*)(void *, Args...)>(vTable[I])(p, args...);
}

inline void **vtable_get(void *obj)
{
	return *reinterpret_cast<void ***>(obj);
}