#pragma once

#include "../definitions/fnv.h"
#include "../definitions/datatable.h"
#include <cctype>
#include <cstdint>
#include <string>
#include <unordered_map>
#include "../interfaces/interfaces.h"
#include <format>

/*
Used this tutorial for the netvar manager
https://www.youtube.com/watch?v=VCsNZ0GRVzo
*/

inline std::unordered_map<uint32_t, uint32_t> netvars;

#define NETVAR(func_name, netvar, type) type& func_name() \
{ \
	static auto offset = netvars[fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
} \

inline void Dump(const char* baseClass, RecvTable* table, uint32_t offset = 0)
{
	for (int i = 0; i < table->propsCount; ++i)
	{
		const auto prop = &table->props[i];
		if (!prop || std::isdigit(prop->varName[0]))
			continue;

		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;

		if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->tableName[0] == 'D')
			Dump(baseClass, prop->dataTable, offset + prop->offset);

		auto netvarName = baseClass + std::string("->") + prop->varName;
		netvars[fnv::Hash(netvarName.c_str())] = offset + prop->offset;
	}
}

inline void SetupNetVars()
{
	interfaces::vstdlib->ConsoleColorPrintf(Color_t{0, 255, 255, 255}, "Dumping netvars...\n");

	for (auto clientClass = interfaces::baseClientDll->GetAllClasses(); clientClass; clientClass = clientClass->next)
		// check if table valid
		if (clientClass->recvTable)
			Dump(clientClass->networkName, clientClass->recvTable);

	interfaces::vstdlib->ConsoleColorPrintf(Color_t{150, 255, 150, 255}, "Netvars dumped!\n");
}