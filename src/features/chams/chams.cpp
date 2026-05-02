#include "chams.h"

#include <unordered_set>

#include "../../sdk/interfaces/interfaces.h"

#include "../../settings/settings.h"
#include "../entitylist/entitylist.h"
#include "../esp/esp_utils.h"

#include "../materialregistry/reg.h"

static std::unordered_set<int> hidden_entities;

static bool is_drawing = false;

void Chams::Init()
{
	Config.chams.active_materials.emplace_back("basic flat");
	Config.chams.active_materials.emplace_back("basic shaded");
}

void Chams::OnLevelPostEntity()
{
	hidden_entities.reserve(interfaces::EntityList->GetHighestEntityIndex());
}

void Chams::OnLevelShutdown()
{
	Reset();
}

bool Chams::ShouldHide(int entindex)
{
	return hidden_entities.find(entindex) != hidden_entities.end();
}

void Chams::OnDoPostScreenSpaceEffects(CTFPlayer* pLocal)
{
	constexpr int DRAW_FLAGS = STUDIO_RENDER | STUDIO_NOSHADOWS;

	Reset();

	if (!Config.chams.enabled)
		return;

	if (pLocal == nullptr)
		return;

	if (Config.chams.active_materials.empty())
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	float savedColor[3];
	float savedBlend;

	savedBlend = interfaces::RenderView->GetBlend();
	interfaces::RenderView->GetColorModulation(savedColor);

	for (auto& ent : EntityList::GetEntities())
	{
		if (!IsValidEntity(pLocal, ent))
			continue;

		ApplyMaterials(ent.ptr, DRAW_FLAGS);
	}

	interfaces::RenderView->SetBlend(savedBlend);
	interfaces::RenderView->SetColorModulation(savedColor);
	interfaces::ModelRender->ForcedMaterialOverride(nullptr);
}

static void DoAttachmentColorModulation(CBaseEntity* attachment, const Color& orig_color)
{
	bool highlight_weapons = Config.esp.packed.weapon;

	if (attachment->IsWeapon() && highlight_weapons)
	{
		float color[3]
		{
			Config.colors.weapon.r()/255.0f,
			Config.colors.weapon.g()/255.0f,
			Config.colors.weapon.b()/255.0f,
		};

		interfaces::RenderView->SetColorModulation(color);
	}
	else
	{
		float color[3]
		{
			orig_color.r()/255.0f,
			orig_color.g()/255.0f,
			orig_color.b()/255.0f,
		};

		interfaces::RenderView->SetColorModulation(color);
	}
}

static void DrawAttachments(CBaseEntity* entity, int drawflags, const Color& orig_color)
{
	if (entity == nullptr)
		return;

	constexpr int MAX_PASSES = 32;
	IClientRenderable* child = entity->FirstShadowChild();

	if (child == nullptr)
		return;

	int passes = 0;

	while (child && passes < MAX_PASSES)
	{
		if (!child->ShouldDraw())
		{
			child = child->NextShadowPeer();
			passes++;
			continue;
		}

		IClientUnknown* unknown = child->GetIClientUnknown();
		CBaseEntity* attachment = unknown ? unknown->GetBaseEntity() : nullptr;

		if (attachment == nullptr)
		{
			child = child->NextShadowPeer();
			passes++;
			continue;
		}

		DoAttachmentColorModulation(attachment, orig_color);

		int entindex = attachment->GetIndex();

		// fixes invisible viewmodel
		if (entindex > 0)
			hidden_entities.insert(entindex);

		child->DrawModel(drawflags);

		passes++;
		child = child->NextShadowPeer();
	}
}

static void DrawEntityAndAttachments(CBaseEntity* entity, int drawflags)
{
	if (entity == nullptr)
		return;

	Color color = ESP_Utils::GetEntityColor(entity);
	float clr[3]
	{
		color.r()/255.0f,
		color.g()/255.0f,
		color.b()/255.0f,
	};

	interfaces::RenderView->SetColorModulation(clr);

	hidden_entities.insert(entity->GetIndex());

	entity->DrawModel(drawflags);
	DrawAttachments(entity, drawflags, color);
}

void Chams::ApplyMaterials(CBaseEntity* entity, int drawflags)
{
	for (const auto& mat_name : Config.chams.active_materials)
	{
		const auto& mat = MaterialRegistry::GetMaterialByName(mat_name);

		if (!mat || !mat->IsValidMat())
			continue;

		interfaces::RenderView->SetBlend(mat->GetAlpha());
		interfaces::ModelRender->ForcedMaterialOverride(mat->GetMaterial());

		is_drawing = true;
		DrawEntityAndAttachments(entity, drawflags);
		is_drawing = false;
	}
}

bool Chams::IsDrawing()
{
	return is_drawing;
}

bool Chams::IsValidEntity(CTFPlayer* pLocal, const EntityListEntry& entry)
{
	if (entry.ptr == nullptr)
		return false;

	if (!(entry.flags & (EntityFlags::IsAlive | EntityFlags::IsPlayer | EntityFlags::IsBuilding)))
		return false;

	if (!entry.ptr->ShouldDraw())
		return false;

	return ESP_Utils::IsValidEntity(pLocal, entry);
}

void Chams::Reset()
{
	hidden_entities.clear();
	is_drawing = false;
}

void Chams::OnGameShutdown()
{
	Config.chams.active_materials.clear();
}