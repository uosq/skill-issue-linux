#include "netvar.h"

/*
Used this tutorial for the netvar manager
https://www.youtube.com/watch?v=VCsNZ0GRVzo
*/

std::vector<NetvarClassEntry> Netvars::m_netvarUIVector;
std::unordered_map<uint32_t, uint32_t> Netvars::m_netvarMap;

void Netvars::Dump(const char* baseClass, RecvTable* table, uint32_t offset)
{
	// find or create class entry
	auto it = std::find_if(m_netvarUIVector.begin(), m_netvarUIVector.end(),
		[&](const NetvarClassEntry& e) { return e.className == baseClass; });

	if (it == m_netvarUIVector.end())
	{
		m_netvarUIVector.push_back({ baseClass, {} });
		it = std::prev(m_netvarUIVector.end());
	}

	for (int i = 0; i < table->propsCount; ++i)
	{
		const auto prop = &table->props[i];
		if (!prop || std::isdigit(prop->varName[0]))
			continue;

		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;

		if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->tableName[0] == 'D')
			Dump(baseClass, prop->dataTable, offset + prop->offset);

		std::string netvarName = std::string(baseClass) + "->" + prop->varName;
		uint32_t finalOffset = offset + prop->offset;

		// for the netvar macro
		m_netvarMap[fnv::Hash(netvarName.c_str())] = finalOffset;

		// for imgui
		it->members.push_back(prop->varName + std::string(" = 0x") + std::to_string(finalOffset));
	}
}

void Netvars::DumpToFile(const char* baseClass, RecvTable* table, std::ofstream& file, uint32_t offset)
{
	for (int i = 0; i < table->propsCount; ++i)
	{
		const auto prop = &table->props[i];
		if (!prop || std::isdigit(prop->varName[0]))
			continue;

		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;

		if (prop->recvType == SendPropType::DATATABLE &&
			prop->dataTable &&
			prop->dataTable->tableName[0] == 'D')
		{
			DumpToFile(baseClass, prop->dataTable, file, offset + prop->offset);
		}

		std::string netvarName = std::string(baseClass) + "->" + prop->varName;
		uint32_t finalOffset = offset + prop->offset;

		// Store for NETVAR macro
		m_netvarMap[fnv::Hash(netvarName.c_str())] = finalOffset;

		// Dump to file
		file << netvarName
		     << " = 0x"
		     << std::hex << finalOffset
		     << std::dec << '\n';
	}
}

// Use this to dump the the netvars to TF2's root folder!
void Netvars::SetupToFile(const char* fileName)
{
	interfaces::Cvar->ConsoleColorPrintf(
		Color_t{0, 255, 255, 255},
		"Dumping netvars...\n"
	);

	std::ofstream file(fileName);
	if (!file.is_open())
	{
		interfaces::Cvar->ConsoleColorPrintf(
			Color_t{255, 0, 0, 255},
			"Failed to open netvars.txt\n"
		);
		return;
	}

	for (auto clientClass = interfaces::ClientDLL->GetAllClasses();
	     clientClass;
	     clientClass = clientClass->next)
	{
		if (clientClass->recvTable)
		{
			file << "\n[" << clientClass->networkName << "]\n";
			DumpToFile(clientClass->networkName, clientClass->recvTable, file);
		}
	}

	file.close();

	interfaces::Cvar->ConsoleColorPrintf(
		Color_t{150, 255, 150, 255},
		"Netvars dumped to file!\n"
	);
}

void Netvars::Setup()
{
	//interfaces::cvar->ConsoleColorPrintf(Color_t{0, 255, 255, 255}, "Dumping netvars...\n");

	for (auto clientClass = interfaces::ClientDLL->GetAllClasses(); clientClass; clientClass = clientClass->next)
		// check if table valid
		if (clientClass->recvTable)
			Dump(clientClass->networkName, clientClass->recvTable);

	//interfaces::cvar->ConsoleColorPrintf(Color_t{150, 255, 150, 255}, "Netvars dumped!\n");
}

uint32_t Netvars::GetNetvarOffset(std::string name)
{
	return m_netvarMap[fnv::Hash(name.c_str())];
}