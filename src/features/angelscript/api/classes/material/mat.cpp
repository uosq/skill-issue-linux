#include "mat.h"

#include "../../../../../sdk/MaterialManager/materialmanager.h"
#include "../../../../../sdk/definitions/imaterial.h"
#include "../../../../logs/logs.h"

#define MAT_CLASSNAME "Material"

// fuck you C++
#define INVALID_MAT "Invalid Material"

void Mat_AddRef(IMaterial *mat)
{
}
void Mat_Release(IMaterial *mat)
{
}

std::string GetName(IMaterial *mat)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! (mat->GetName)");
		return INVALID_MAT;
	}

	return mat->GetName();
}

std::string GetTextureGroupName(IMaterial *mat)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! "
			    "(mat->GetTextureGroupName)");
		return INVALID_MAT;
	}

	return mat->GetTextureGroupName();
}

bool IsTranslucent(IMaterial *mat)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! (mat->IsTranslucent)");
		return false;
	}
	return mat->IsTranslucent();
}

void SetMatVarFlag(IMaterial *mat, int flag, bool on)
{
	if (!mat)
		return Logs::Error("AS: Calling on a invalid material! "
				   "(mat->SetMaterialVarFlag)");
	mat->SetMaterialVarFlag(static_cast<MaterialVarFlags_t>(flag), on);
}

bool GetMatVarFlag(IMaterial *mat, int flag)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! "
			    "(mat->GetMaterialVarFlag)");
		return false;
	}

	return mat->GetMaterialVarFlag(static_cast<MaterialVarFlags_t>(flag));
}

void SetAlphaMod(IMaterial *mat, float alpha)
{
	if (!mat)
		return Logs::Error("AS: Calling on a invalid material! "
				   "(mat->SetAlphaModulation)");
	mat->AlphaModulate(alpha);
}

void SetColorMod(IMaterial *mat, float r, float g, float b)
{
	if (!mat)
		return Logs::Error("AS: Calling on a invalid material! "
				   "(mat->SetAlphaModulation)");
	mat->ColorModulate(r, g, b);
}

float GetAlphaMod(IMaterial *mat)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! "
			    "(mat->GetAlphaModulation)");
		return 0.0f;
	}

	return mat->GetAlphaModulation();
}

void GetColorMod(IMaterial *mat, float &r, float &g, float &b)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! "
			    "(mat->GetAlphaModulation)");
		return;
	}

	mat->GetColorModulation(&r, &g, &b);
}

bool DeleteMat(IMaterial *mat)
{
	if (!mat)
	{
		Logs::Error("AS: Calling on a invalid material! (mat->Delete)");
		return false;
	}
	return MaterialManager::FreeMaterial(mat->GetName());
}

void Material_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(MAT_CLASSNAME, 0, asOBJ_REF);

	engine->RegisterObjectBehaviour(MAT_CLASSNAME, asBEHAVE_ADDREF, "void f()", asFUNCTION(Mat_AddRef),
					asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour(MAT_CLASSNAME, asBEHAVE_RELEASE, "void f()", asFUNCTION(Mat_Release),
					asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectMethod(MAT_CLASSNAME, "string GetName()", asFUNCTION(GetName), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "string GetTextureGroupName()", asFUNCTION(GetTextureGroupName),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "bool IsTranslucent() const", asFUNCTION(IsTranslucent),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "void SetMaterialVarFlag(int flag, bool on)",
				     asFUNCTION(SetMatVarFlag), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "bool GetMaterialVarFlag(int flag)", asFUNCTION(GetMatVarFlag),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "void SetColorModulation(float r, float g, float b)",
				     asFUNCTION(SetColorMod), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "void SetAlphaModulation(float a)", asFUNCTION(SetAlphaMod),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "float GetAlphaModulation()", asFUNCTION(GetAlphaMod),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME,
				     "void GetColorModulation(float &out r, "
				     "float &out g, float &out b)",
				     asFUNCTION(GetColorMod), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "bool IsErrorMaterial()", asFUNCTION(IsErrorMaterial),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(MAT_CLASSNAME, "bool Delete()", asFUNCTION(DeleteMat), asCALL_CDECL_OBJFIRST);
}