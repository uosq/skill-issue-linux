#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/definitions/imaterial.h"
#include "../../sdk/definitions/itexture.h"
#include "../../sdk/definitions/imaterialsystem.h"
#include "../../sdk/interfaces/interfaces.h"
#include <unordered_map>
#include <vector>

class MaterialManager
{
private:
	std::unordered_map<std::string, IMaterial*> m_Materials;
	std::unordered_map<std::string, ITexture*> m_Textures;

public:
	IMaterial* CreateMaterial(const std::string& name, const std::string& vmt);
	bool FreeMaterial(const std::string& name);
	bool MaterialExists(const std::string& name);

	ITexture* CreateTextureNamedRenderTarget(const std::string& name, int width, int height);
	bool FreeTexture(const std::string& name);
	bool TextureExists(const std::string& name);
};

inline IMaterial* MaterialManager::CreateMaterial(const std::string& name, const std::string& vmt)
{
	auto it = m_Materials.find(name);
	if (it != m_Materials.end())
		return it->second;

	KeyValues* kv = new KeyValues(name.c_str());
	kv->LoadFromBuffer(name.c_str(), vmt.c_str());

	IMaterial* mat = interfaces::MaterialSystem->CreateMaterial(name.c_str(), kv);
	mat->IncrementReferenceCount();

	if (mat == nullptr)
		return nullptr;

	m_Materials.insert(std::pair<std::string, IMaterial*>(name, mat));
	return mat;
}

inline bool MaterialManager::FreeMaterial(const std::string& name)
{
	auto it = m_Materials.find(name);
	if (it == m_Materials.end())
		return false;

	IMaterial* mat = it->second;
	if (mat == nullptr)
		return false;

	mat->DecrementReferenceCount();
	mat->DeleteIfUnreferenced();
	m_Materials.erase(it);

	return true;
}

inline bool MaterialManager::MaterialExists(const std::string& name)
{
	auto it = m_Materials.find(name);
	if (it == m_Materials.end())
		return false;

	IMaterial* mat = it->second;
	if (mat == nullptr)
		return false;

	return true;
}

inline ITexture* MaterialManager::CreateTextureNamedRenderTarget(const std::string& name, int width, int height)
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
		return it->second;

	ITexture* tex = interfaces::MaterialSystem->CreateNamedRenderTargetTextureEx(
		name.c_str(),
		width, height,
		RT_SIZE_LITERAL,
		IMAGE_FORMAT_RGB888,
		MATERIAL_RT_DEPTH_SHARED,
		TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA,
		CREATERENDERTARGETFLAGS_HDR
	);

	tex->IncrementReferenceCount();

	m_Textures.insert(std::pair<std::string, ITexture*>(name, tex));
	return tex;
}

inline bool MaterialManager::FreeTexture(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it == m_Textures.end())
		return false;

	ITexture* tex = it->second;
	if (tex == nullptr)
		return false;

	tex->DecrementReferenceCount();
	tex->DeleteIfUnreferenced();
	m_Textures.erase(it);

	return true;
}

inline bool MaterialManager::TextureExists(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it == m_Textures.end())
		return false;

	ITexture* tex = it->second;
	if (tex == nullptr)
		return false;

	return true;
}

inline MaterialManager g_MaterialManager;