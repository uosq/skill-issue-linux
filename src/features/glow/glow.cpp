#include "glow.h"

namespace Glow
{
	GlowMaterials m_Materials = {};
	std::vector<CBaseEntity*> glowEnts = {};
	std::unordered_set<int> m_Entities = {};
	bool m_bRunning = false;
	bool m_bInitialized = false;

	void Init()
	{
		if (m_bInitialized)
			return;

		m_Materials.glowColor = interfaces::MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);

		m_Materials.haloAddToScreen = MaterialManager::CreateMaterial("GlowMaterial1",
			"UnlitGeneric\n"
			"{\n"
			"	$basetexture \"GlowBuffer1\"\n"
			"	$additive \"1\"\n"
			"}"
		);

		m_Materials.blurX = MaterialManager::CreateMaterial("GlowMatBlurX", 
			"BlurFilterX\n"
			"{\n"
			"	$basetexture \"GlowBuffer1\"\n"
			"}"
		);

		m_Materials.blurY = MaterialManager::CreateMaterial("GlowMatBlurY",
			"BlurFilterY\n"
			"{\n"
			"	$basetexture \"GlowBuffer2\"\n"
			"}"
		);

		m_Materials.pRtFullFrame = interfaces::MaterialSystem->FindTexture("_rt_FullFrameFB", "RenderTargets", true);

		m_Materials.glowBuffer1 = MaterialManager::CreateTextureNamedRenderTarget("GlowBuffer1", m_Materials.pRtFullFrame->GetActualWidth(), m_Materials.pRtFullFrame->GetActualHeight());
		m_Materials.glowBuffer2 = MaterialManager::CreateTextureNamedRenderTarget("GlowBuffer2", m_Materials.pRtFullFrame->GetActualWidth(), m_Materials.pRtFullFrame->GetActualHeight());

		m_bInitialized = true;
	}

	bool ShouldHide(int entindex)
	{
		return m_Entities.find(entindex) != m_Entities.end();
	}

	void DrawEntities()
	{
		for (auto ent : glowEnts)
		{
			Color color = ESP_Utils::GetEntityColor(ent);
			float mod[3] = {color.r()/255.0f, color.g()/255.0f, color.b()/255.0f};
			interfaces::RenderView->SetColorModulation(mod);
			m_bRunning = true;
			ent->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);

			auto moveChild = ent->FirstShadowChild();
			int passes = 0;
			while (moveChild != nullptr && passes <= 32)
			{
				if (Settings::ESP::weapon && static_cast<CBaseEntity*>(moveChild)->IsWeapon())
				{
					color = Settings::Colors::weapon;
					// this is fucking stupid
					// why is a array not assignable?
					mod[0] = color.r()/255.0f;
					mod[1] = color.g()/255.0f;
					mod[2] = color.b()/255.0f;
					interfaces::RenderView->SetColorModulation(mod);
				}

				moveChild->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
				moveChild = moveChild->NextShadowPeer();
				passes++;
			}
			m_bRunning = false;
		}
	}

	void GetEntities()
	{
		for (auto entry : EntityList::GetEntities())
		{
			CBaseEntity* entity = entry.ptr;

			if (!entity->ShouldDraw())
				continue;

			if (!(entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsBuilding)))
				continue;

			if (entry.flags & EntityFlags::IsBuilding)
			{
				CBaseObject* building = static_cast<CBaseObject*>(entity);
				if (building == nullptr)
					continue;

				if (building->m_iHealth() <= 0)
					continue;
			}

			glowEnts.emplace_back(entity);
		}
	}

	// Call in DoPostScreenSpaceEffects
	void Run()
	{
		m_bRunning = false;
		if (!m_Entities.empty())
			m_Entities.clear();
		if (!glowEnts.empty())
			glowEnts.clear();

		if (Settings::ESP::blur == 0 && Settings::ESP::stencil == 0)
			return;

		if (interfaces::Engine->IsTakingScreenshot())
			return;
		
		auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
		if (!pRenderContext)
			return;

		GetEntities();
		
		if (glowEnts.empty())
			return;

		int w, h;
		interfaces::Engine->GetScreenSize(w, h);

		{	// stencil pass
			pRenderContext->SetStencilEnable(true);
			float savedBlend = interfaces::RenderView->GetBlend();
			interfaces::ModelRender->ForcedMaterialOverride(m_Materials.glowColor);
			interfaces::RenderView->SetBlend(0);

			pRenderContext->SetStencilReferenceValue(1);
			pRenderContext->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_ALWAYS);
			pRenderContext->SetStencilPassOperation(STENCILOPERATION_REPLACE);
			pRenderContext->SetStencilFailOperation(STENCILOPERATION_KEEP);
			pRenderContext->SetStencilZFailOperation(STENCILOPERATION_REPLACE);

			DrawEntities();

			interfaces::ModelRender->ForcedMaterialOverride(nullptr);
			interfaces::RenderView->SetBlend(savedBlend);
			pRenderContext->SetStencilEnable(false);
		}

		{	// color pass
			pRenderContext->PushRenderTargetAndViewport();

			float savedColor[3], savedBlend;
			interfaces::RenderView->GetColorModulation(savedColor);
			savedBlend = interfaces::RenderView->GetBlend();

			interfaces::RenderView->SetBlend(1.0f);
			pRenderContext->SetRenderTarget(m_Materials.glowBuffer1);
			pRenderContext->Viewport(0, 0, w, h);
			pRenderContext->ClearColor3ub(0, 0, 0);
			pRenderContext->ClearBuffers(true, false, false);
			interfaces::ModelRender->ForcedMaterialOverride(m_Materials.glowColor);

			DrawEntities();

			interfaces::ModelRender->ForcedMaterialOverride(nullptr);
			interfaces::RenderView->SetBlend(savedBlend);
			interfaces::RenderView->SetColorModulation(savedColor);

			pRenderContext->PopRenderTargetAndViewport();
		}

		// blur pass
		if (Settings::ESP::blur > 0)
		{
			pRenderContext->PushRenderTargetAndViewport();
			pRenderContext->Viewport(0, 0, w, h);

			for (int i = 0; i < Settings::ESP::blur; ++i)
			{
				pRenderContext->SetRenderTarget(m_Materials.glowBuffer2);
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.blurX, 0, 0, w, h, 0, 0, w - 1, h - 1, w, h);
				pRenderContext->SetRenderTarget(m_Materials.glowBuffer1);
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.blurY, 0, 0, w, h, 0, 0, w - 1, h - 1, w, h);
			}

			pRenderContext->PopRenderTargetAndViewport();
		}

		{	// draw pass
			pRenderContext->SetStencilEnable(true);
			pRenderContext->SetStencilWriteMask(0);
			pRenderContext->SetStencilTestMask(0xFF);

			pRenderContext->SetStencilReferenceValue(1);
			pRenderContext->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_NOTEQUAL);

			pRenderContext->SetStencilPassOperation(STENCILOPERATION_KEEP);
			pRenderContext->SetStencilFailOperation(STENCILOPERATION_KEEP);
			pRenderContext->SetStencilZFailOperation(STENCILOPERATION_KEEP);

			// this is from amalgam
			if (Settings::ESP::stencil)
			{
				int side = (Settings::ESP::stencil + 1) / 2;
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, -side, 0, w, h, 0, 0, w - 1, h - 1, w, h);
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, 0, -side, w, h, 0, 0, w - 1, h - 1, w, h);
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, side, 0, w, h, 0, 0, w - 1, h - 1, w, h);
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, 0, side, w, h, 0, 0, w - 1, h - 1, w, h);
				
				int corner = Settings::ESP::stencil / 2;
				if (corner)
				{
					pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, -corner, -corner, w, h, 0, 0, w - 1, h - 1, w, h);
					pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, corner, corner, w, h, 0, 0, w - 1, h - 1, w, h);
					pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, corner, -corner, w, h, 0, 0, w - 1, h - 1, w, h);
					pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, -corner, corner, w, h, 0, 0, w - 1, h - 1, w, h);
				}
			}

			if (Settings::ESP::blur)
				pRenderContext->DrawScreenSpaceRectangle(m_Materials.haloAddToScreen, 0, 0, w, h, 0, 0, w - 1, h - 1, w, h);

			pRenderContext->SetStencilEnable(false);
		}
	}
}