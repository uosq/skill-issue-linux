#include "chamsmaterial.h"

#include "../../sdk/MaterialManager/materialmanager.h"

ChamsMaterial::ChamsMaterial(const std::string& name, const std::string& vmt)
: m_DisplayName(name), m_InternalName(name), m_Vmt(vmt), m_Alpha(1.0f)
{
	m_Mat = MaterialManager::CreateMaterial(name, vmt);
}

ChamsMaterial::~ChamsMaterial()
{
	if (m_Mat == nullptr)
		return;

	if (!MaterialManager::MaterialExists(m_InternalName))
		return;

	MaterialManager::FreeMaterial(m_InternalName);
	m_Mat = nullptr;
}

const std::string& ChamsMaterial::GetVMT()
{
	return m_Vmt;
}

void ChamsMaterial::SetVMT(const std::string& vmt)
{
	this->m_Vmt = vmt;
}

void ChamsMaterial::Refresh()
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

bool ChamsMaterial::IsValidMat()
{
	return m_Mat && !m_Mat->IsErrorMaterial();
}

void ChamsMaterial::SetUsed(bool used)
{
	m_Used = used;
}

bool ChamsMaterial::IsUsed()
{
	return m_Used;
}

IMaterial* ChamsMaterial::GetMaterial()
{
	return m_Mat;
}

const std::string& ChamsMaterial::GetInternalName()
{
	return m_InternalName;
}

void ChamsMaterial::SetInternalName(const std::string& name)
{
	m_InternalName = name;
}

const std::string& ChamsMaterial::GetDisplayName()
{
	return m_DisplayName;
}

void ChamsMaterial::SetDisplayName(const std::string& name)
{
	m_DisplayName = name;
}

float ChamsMaterial::GetAlpha()
{
	return m_Alpha;
}

void ChamsMaterial::SetAlpha(float alpha)
{
	m_Alpha = alpha;
}