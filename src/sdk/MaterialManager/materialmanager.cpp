#include "materialmanager.h"

std::unordered_map<std::string, IMaterial*> MaterialManager::m_Materials;
std::unordered_map<std::string, ITexture*> MaterialManager::m_Textures;

IMaterial* MaterialManager::CreateMaterial(const std::string& name, const std::string& vmt)
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

	m_Materials.emplace(std::pair<std::string, IMaterial*>(name, mat));
	return mat;
}

bool MaterialManager::FreeMaterial(const std::string& name)
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

bool MaterialManager::MaterialExists(const std::string& name)
{
	auto it = m_Materials.find(name);
	if (it == m_Materials.end())
		return false;

	IMaterial* mat = it->second;
	if (mat == nullptr)
		return false;

	return true;
}

ITexture* MaterialManager::CreateTextureNamedRenderTarget(const std::string& name, int width, int height)
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

bool MaterialManager::FreeTexture(const std::string& name)
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

bool MaterialManager::TextureExists(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it == m_Textures.end())
		return false;

	ITexture* tex = it->second;
	if (tex == nullptr)
		return false;

	return true;
}

IMaterial* MaterialManager::GetMaterial(const std::string& name)
{
	auto it = m_Materials.find(name);
	if (it == m_Materials.end())
		return nullptr;

	IMaterial* mat = it->second;
	if (mat == nullptr)
		return nullptr;

	return mat;
}

ITexture* MaterialManager::GetTexture(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it == m_Textures.end())
		return nullptr;

	ITexture* tex = it->second;
	if (tex == nullptr)
		return nullptr;

	return tex;
}

void MaterialManager::Init(void)
{
	m_Materials.reserve(10);
	m_Textures.reserve(10);
}

void MaterialManager::Unitialize(void)
{
	while (!m_Materials.empty())
		FreeMaterial(m_Materials.begin()->first);

	while (!m_Textures.empty())
		FreeTexture(m_Textures.begin()->first);

	m_Materials.clear();
	m_Textures.clear();
}