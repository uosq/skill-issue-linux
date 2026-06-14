#pragma once

#include <string>

#include "../../sdk/definitions/imaterial.h"
#include "../../sdk/definitions/keyvalues.h"

class CustomMaterial
{
public:
	CustomMaterial(const std::string& name, const std::string& vmt)
	: m_iSlotIndex(-1), m_Alpha(1.0f), m_DisplayName(name), m_InternalName(name), m_Vmt(vmt)
	{
		CreateValveMaterial();
	}

	~CustomMaterial()
	{
		DestroyValveMaterial();
	}

	void CreateValveMaterial()
	{
		if (m_Vmt.empty()) return;

		KeyValues* kv = new KeyValues("");
		kv->LoadFromBuffer(m_InternalName.c_str(), m_Vmt.c_str());

		m_Mat = interfaces::MaterialSystem->CreateMaterial(m_InternalName.c_str(), kv);

		if (m_Mat)
			m_Mat->IncrementReferenceCount();
	}

	void DestroyValveMaterial()
	{
		if (!m_Mat) return;

		m_Mat->DecrementReferenceCount();
		m_Mat->DeleteIfUnreferenced();
		m_Mat = nullptr;
	}

	void Refresh()
	{
		DestroyValveMaterial();

		m_InternalName = m_DisplayName + "_" + std::to_string(GetNextNumber());
		CreateValveMaterial();
	}

	bool IsValidMat() const { return m_Mat && !m_Mat->IsErrorMaterial(); }
	IMaterial* GetMaterial() const { return m_Mat; }

	const std::string& GetVMT() const { return m_Vmt; }
	void SetVMT(const std::string& vmt) { m_Vmt = vmt; }

	const std::string& GetInternalName() const { return m_InternalName; }
	const std::string& GetDisplayName() const { return m_DisplayName; }
	void SetDisplayName(const std::string& name) { m_DisplayName = name; m_InternalName = name; }

	float GetAlpha() const { return m_Alpha; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }

	int m_iSlotIndex { -1 };

private:
	int GetNextNumber()
	{
		static int last_number { 0 };
		return last_number++;
	}

private:
	float m_Alpha { 1.0f };
	std::string m_DisplayName {};
	std::string m_InternalName {};
	std::string m_Vmt {};
	IMaterial* m_Mat = nullptr;
};