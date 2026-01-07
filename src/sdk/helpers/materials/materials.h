#pragma once

#include <string>
#include "../../interfaces/interfaces.h"
#include "../../definitions/keyvalues.h"

namespace helper
{
	namespace material
	{
		inline void CreateMaterial(std::string name, std::string type, std::string vmt)
		{
			KeyValues *kv = new KeyValues(type.c_str());
			kv->LoadFromBuffer(name.c_str(), vmt.c_str());
			interfaces::MaterialSystem->CreateMaterial(name.c_str(), kv);
		}
	}
}