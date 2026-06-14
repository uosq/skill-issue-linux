#include "materialmanager.h"
#include "custom_material.h"

#include <filesystem>
#include <fstream>
#include <optional>

#include "../../sdk/definitions/fnv.h"

bool CMaterialManager::DoesMaterialExist(const std::string& displayName)
{
	for (const auto& mat : m_Materials)
	{
		if (mat->GetDisplayName() == displayName)
			return true;
	}
	return false;
}

bool CMaterialManager::AddMaterial(const std::string& displayName, const std::string& vmt, std::shared_ptr<CustomMaterial>& out)
{
	if (DoesMaterialExist(displayName))
		return false;

	m_Materials.emplace_back(std::make_shared<CustomMaterial>(displayName, vmt));
	out = m_Materials.back();
	return true;
}

bool CMaterialManager::RemoveMaterial(const std::string& displayName)
{
	for (auto it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		if ((*it)->GetDisplayName() == displayName)
		{
			m_Materials.erase(it);
			return true;
		}
	}
	return false;
}

std::shared_ptr<CustomMaterial> CMaterialManager::GetMaterialByName(const std::string& displayName)
{
	for (const auto& mat : m_Materials)
	{
		if (mat->GetDisplayName() == displayName)
			return mat;
	}
	return nullptr;
}

std::vector<std::shared_ptr<CustomMaterial>>& CMaterialManager::GetMaterials()
{
	return m_Materials;
}

void CMaterialManager::Init()
{
	m_Materials.reserve(20);

	std::shared_ptr<CustomMaterial> dummy;
	AddMaterial("basic flat", "UnlitGeneric\n{\n\t$basetexture \"white\"\n}", dummy);
	AddMaterial("basic shaded", "VertexLitGeneric\n{\n\t$basetexture \"white\"\n}", dummy);

	LoadMaterials();
}

void CMaterialManager::Uninitialize()
{
	SaveToDirectory(MATERIAL_DIR);

	m_Materials.clear();

	while (!m_Textures.empty())
		FreeTexture(m_Textures.begin()->first);
	m_Textures.clear();
}

void CMaterialManager::LoadMaterials()
{
	LoadMaterialsFromDirectory("chams");
	LoadMaterialsFromDirectory("backtrack");
}

void CMaterialManager::LoadMaterialsFromDirectory(const std::string& dir_name)
{
        std::string folder_path = std::string(MATERIAL_DIR) + dir_name;
        if (!std::filesystem::exists(folder_path)) 
                std::filesystem::create_directories(folder_path);

        for (const auto& entry : std::filesystem::directory_iterator(folder_path))
        {
                if (entry.path().extension() == ".vmt")
                {
                        std::string file_stem = entry.path().stem().string(); 
                        std::string unique_display_name = dir_name + "/" + file_stem;

                        std::ifstream file(entry.path());
                        std::stringstream buffer;
                        buffer << file.rdbuf();
                        std::string vmt_content = buffer.str();

                        std::shared_ptr<CustomMaterial> mat = GetMaterialByName(unique_display_name);
                        if (!mat)
                        {
                                AddMaterial(unique_display_name, vmt_content, mat);
                        }
                        else
                        {
                                mat->SetVMT(vmt_content);
                                mat->Refresh();
                        }
                }
        }

        // AFTER LOADING ALL MATERIALS: Re-assign sequence bit slots cleanly (0-31)
        for (size_t i = 0; i < m_Materials.size(); ++i)
        {
                m_Materials[i]->m_iSlotIndex = static_cast<int>(i);
        }
}

bool CMaterialManager::SaveToDirectory(const std::string& dirPath)
{
	for (const auto& mat : m_Materials)
	{
		if (mat->GetDisplayName() == "basic flat" || mat->GetDisplayName() == "basic shaded")
			continue;

		std::filesystem::path filePath = std::filesystem::path(dirPath) / (mat->GetDisplayName() + ".vmt");

		std::filesystem::create_directories(filePath.parent_path());

		std::ofstream file(filePath);
		if (!file.is_open())
			continue;

		file << mat->GetVMT();
	}
	return true;
}

ITexture* CMaterialManager::CreateTextureNamedRenderTarget(const std::string& name, int width, int height)
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
		return it->second;

	ITexture* tex = interfaces::MaterialSystem->CreateNamedRenderTargetTextureEx
	(
		name.c_str(), width, height, RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED,
		TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA, CREATERENDERTARGETFLAGS_HDR
	);

	if (tex)
	{
		tex->IncrementReferenceCount();
		m_Textures.insert({name, tex});
	}
	return tex;
}

bool CMaterialManager::FreeTexture(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it == m_Textures.end()) return false;

	ITexture* tex = it->second;
	if (tex)
	{
		tex->DecrementReferenceCount();
		tex->DeleteIfUnreferenced();
	}
	m_Textures.erase(it);
	return true;
}

bool CMaterialManager::TextureExists(const std::string& name)
{
	return m_Textures.find(name) != m_Textures.end();
}

ITexture* CMaterialManager::GetTexture(const std::string& name)
{
	auto it = m_Textures.find(name);
	return (it != m_Textures.end()) ? it->second : nullptr;
}

std::optional<std::shared_ptr<CustomMaterial>> CMaterialManager::FindMaterial(uint32_t hash)
{
	for (const auto& mat : m_Materials)
	{
		if (fnv::Hash(mat->GetDisplayName().c_str()) == hash)
			return mat;
	}

	return std::nullopt;
}