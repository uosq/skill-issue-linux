#include "../sdk/definitions/imaterial.h"

#include "../features/hookmanager/hookmanager.hpp"
#include "../features/MaterialManager/materialmanager.h"

INIT_HOOK(CMaterial_Uncache, void, (IMaterial* mat, bool bPreserveVars), "materialsystem.so", "55 48 89 E5 41 56 41 55 41 54 49 89 FC 53 89 F3 48 81 EC 10 01 00 00")
{
	const char* name = mat->GetName();

	if (name == nullptr) return;

	if (features::materials.DoesMaterialExist(name))
		return;

	original_CMaterial_Uncache(mat, bPreserveVars);
}
