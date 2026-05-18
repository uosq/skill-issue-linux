#pragma once

#include <cstdint>

inline uintptr_t RelToAbs(uintptr_t instr, int dispOffset = 3, int instrSize = 7)
{
	int32_t disp = *reinterpret_cast<int32_t*>(instr + dispOffset);
	return instr + instrSize + disp;
}

template <std::size_t Index, typename Ret, typename... Args> inline Ret vtable_call(void *p, Args... args)
{
	auto vTable = *static_cast<void ***>(p);
	return reinterpret_cast<Ret (*)(void *, Args...)>(vTable[Index])(p, args...);
}

template <std::size_t Index, typename Ret, typename... Args> inline Ret vtable_call(uintptr_t u, Args... args)
{
	auto p	    = reinterpret_cast<void *>(u);
	auto vTable = *static_cast<void ***>(p);
	return reinterpret_cast<Ret (*)(void *, Args...)>(vTable[Index])(p, args...);
}

inline void **vtable_get(void *obj)
{
	return *reinterpret_cast<void ***>(obj);
}