#pragma once

#include <string>
#include "../../interfaces/interfaces.h"
#include "../../definitions/keyvalues.h"
#include "../../definitions/imaterial.h"

namespace helper
{
	namespace material
	{
		inline IMaterial* CreateMaterial(std::string name, std::string vmt)
		{
			KeyValues *kv = new KeyValues("");
			kv->LoadFromBuffer("", vmt.c_str());
			IMaterial* mat = interfaces::MaterialSystem->CreateMaterial(name.c_str(), kv);
			mat->AddRef();
			return mat;
		}
	}
}