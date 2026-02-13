#pragma once

#include "../definitions/fnv.h"
#include "../definitions/datatable.h"
#include "../interfaces/interfaces.h"
#include <cctype>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>

/*
Used this tutorial for the netvar manager
https://www.youtube.com/watch?v=VCsNZ0GRVzo
*/

struct NetvarClassEntry
{
    std::string className;
    std::vector<std::string> members;
};

namespace Netvars
{
	extern std::vector<NetvarClassEntry> m_netvarUIVector;
	extern std::unordered_map<uint32_t, uint32_t> m_netvarMap;

	void Setup(void);
	void SetupToFile(const char* fileName);
	void Dump(const char* baseClass, RecvTable* table, uint32_t offset = 0);
	void DumpToFile( const char* baseClass, RecvTable* table, std::ofstream& file, uint32_t offset = 0);

	uint32_t GetNetvarOffset(std::string name);
}

#define NETVAR(func_name, netvar, type) type& func_name() \
{ \
	static auto offset = Netvars::m_netvarMap[fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
}

#define NETVAR_OFFSET(func_name, netvar, type, offset) type& func_name() \
{ \
	static auto m_offset = Netvars::m_netvarMap[fnv::HashConst(netvar)] + offset; \
	return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + m_offset); \
}

#define NETVAR_ARRAY(func_name, netvar, type) type& func_name(int iIndex) \
{ \
	static auto offset = Netvars::m_netvarMap[fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset + iIndex * sizeof(type)); \
}

#define NETVAR_ARRAY_OFF(func_name, netvar, typeoffset) type& func_name(int iIndex) \
{ \
	static int nOffset = Netvars::m_netvarMap[fnv::HashConst(netvar)] + offset; \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset + iIndex * sizeof(type)); \
}
