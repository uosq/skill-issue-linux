#include "custom_material.h"

#include "../../sdk/MaterialManager/materialmanager.h"

CustomMaterial::CustomMaterial(const std::string& name, const std::string& vmt)
: m_DisplayName(name), m_InternalName(name), m_Vmt(vmt), m_Alpha(1.0f)
{
	m_Mat = MaterialManager::CreateMaterial(name, vmt);
}

CustomMaterial::~CustomMaterial()
{
	if (m_Mat == nullptr)
		return;

	if (!MaterialManager::MaterialExists(m_InternalName))
		return;

	MaterialManager::FreeMaterial(m_InternalName);
	m_Mat = nullptr;
}

const std::string& CustomMaterial::GetVMT()
{
	return m_Vmt;
}

void CustomMaterial::SetVMT(const std::string& vmt)
{
	this->m_Vmt = vmt;
}

void CustomMaterial::Refresh()
{
	// delete our material
	// hot reloading is delulu
	// IMaterial->Refresh() doesn't refresh shit
	if (m_Mat)
	{
		MaterialManager::FreeMaterial(m_Mat->GetName());
		m_Mat = nullptr;
	}

	KeyValues* kv = new KeyValues("");
	kv->LoadFromBuffer(m_InternalName.c_str(), m_Vmt.c_str());

	std::string unique = m_InternalName + "_" + std::to_string(rand());

	m_Mat = MaterialManager::CreateMaterial(unique, m_Vmt);
}

bool CustomMaterial::IsValidMat()
{
	return m_Mat && !m_Mat->IsErrorMaterial();
}

IMaterial* CustomMaterial::GetMaterial()
{
	return m_Mat;
}

const std::string& CustomMaterial::GetInternalName()
{
	return m_InternalName;
}

void CustomMaterial::SetInternalName(const std::string& name)
{
	m_InternalName = name;
}

const std::string& CustomMaterial::GetDisplayName()
{
	return m_DisplayName;
}

void CustomMaterial::SetDisplayName(const std::string& name)
{
	m_DisplayName = name;
}

float CustomMaterial::GetAlpha()
{
	return m_Alpha;
}

void CustomMaterial::SetAlpha(float alpha)
{
	m_Alpha = alpha;
}