#pragma once

#include <string>

#include "../../sdk/definitions/imaterial.h"

class CustomMaterial
{
public:
	CustomMaterial(const std::string& name, const std::string& vmt);
	~CustomMaterial();

	const std::string& GetVMT();
	void SetVMT(const std::string& vmt);

	bool IsValidMat();
	IMaterial* GetMaterial();
	void Refresh();

	const std::string& GetInternalName();
	void SetInternalName(const std::string& name);

	const std::string& GetDisplayName();
	void SetDisplayName(const std::string& name);

	float GetAlpha();
	void SetAlpha(float alpha);

private:
	std::string m_DisplayName = "";
	std::string m_InternalName = "";
	std::string m_Vmt = "";
	IMaterial* m_Mat = nullptr;
	float m_Alpha = 1.0f;
};