#include "chams.h"

#include <unordered_set>

#include "../../sdk/interfaces/interfaces.h"

#include "../entitylist/entitylist.h"

#include "../esp/esp_utils.h"
#include "../esp/esp.h"
#include "../colors/colors.h"

#include "../MaterialManager/materialmanager.h"

void Chams::Init()
{

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

	if (!config::chams::enabled.Get())
		return;

	if (pLocal == nullptr)
		return;

	if (config::chams::material.Get() == 0)
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	float savedColor[3];
	float savedBlend;

	savedBlend = interfaces::RenderView->GetBlend();
	interfaces::RenderView->GetColorModulation(savedColor);

	for (auto& ent : features::entities.GetEntities())
	{
		if (!IsValidEntity(pLocal, ent))
			continue;

		ApplyMaterials(ent.ptr, DRAW_FLAGS);
	}

	interfaces::RenderView->SetBlend(savedBlend);
	interfaces::RenderView->SetColorModulation(savedColor);
	interfaces::ModelRender->ForcedMaterialOverride(nullptr);
}

void Chams::DoAttachmentColorModulation(CBaseEntity* attachment, const Color& orig_color)
{
	bool highlight_weapons = config::esp::enabled.Get();

	if (attachment->IsWeapon() && highlight_weapons)
	{
		float color[3]
		{
			config::colors::weapon.Get().r()/255.0f,
			config::colors::weapon.Get().g()/255.0f,
			config::colors::weapon.Get().b()/255.0f,
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

void Chams::DrawAttachments(CBaseEntity* entity, int drawflags, const Color& orig_color)
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

void Chams::DrawEntityAndAttachments(CBaseEntity* entity, int drawflags)
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
	const uint32_t activeMask = config::chams::material.Get();
	if (activeMask == 0)
		return; // No materials selected

	const auto& loadedMaterials = features::materials.GetMaterials();

	is_drawing = true;

	// Loop over all materials and render active ones sequentially
	for (const auto& mat : loadedMaterials)
	{
		if (mat->m_iSlotIndex == -1 || !mat->IsValidMat())
			continue;

		// Check if this specific material's bit is active in the configuration mask
		if ((activeMask & (1u << mat->m_iSlotIndex)) != 0)
		{
			// Apply this layer's properties
			interfaces::RenderView->SetBlend(mat->GetAlpha());
			interfaces::ModelRender->ForcedMaterialOverride(mat->GetMaterial());

			// Render the model pass
			DrawEntityAndAttachments(entity, drawflags);
		}
	}

	is_drawing = false;

	// Reset material override when done rendering all stacked passes
	interfaces::ModelRender->ForcedMaterialOverride(nullptr);
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

}