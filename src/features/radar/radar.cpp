#include "radar.h"

namespace Radar
{
	void DrawContents();
	void DrawHealthbar(ImDrawList* draw, ImVec2 pos, int health, int maxhealth, int iconSize);

	int m_iRange;
	float m_flRadius;

	void Init();

	// Call in EngineVGui->Paint
	void Run();

	float GetRadius();
	int GetRange();
	Vec2 WorldToRadar(const Vector& localPos, const Vector& enemyPos, float viewAnglesYaw);
};

void Radar::Run()
{
	int size = Settings::Radar::size;

	m_iRange = Settings::Radar::range;
	m_flRadius = size * 0.5f;

	// Shouldn't be possible without Lua
	// but just in case a smart ass tries it anyway
	if (size == 0)
		return;

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!Settings::menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Radar", nullptr, flags))
	{
		DrawContents();
		ImGui::Dummy(ImVec2(size, size));
	}

	ImGui::End();
}

Vec2 Radar::WorldToRadar(const Vector& localPos, const Vector& enemyPos, float viewAnglesYaw)
{
	Vec2 delta = {enemyPos.x - localPos.x, enemyPos.y - localPos.y};

	// rotate by -yaw
	float yaw = DEG2RAD(-viewAnglesYaw);
	float cosYaw = std::cos(yaw);
	float sinYaw = std::sin(yaw);

	float rx = delta.x * cosYaw - delta.y * sinYaw;
	float ry = delta.y * cosYaw + delta.x * sinYaw;

	// scale to radar
	float dist = std::sqrt(rx * rx + ry * ry);
	if (dist > m_iRange)
	{
		float s = m_iRange / dist;
		rx *= s;
		ry *= s;
	}

	rx = (rx / m_iRange) * m_flRadius;
	ry = (ry / m_iRange) * m_flRadius;

	return { rx, ry };
}

float Radar::GetRadius()
{
	return m_flRadius;
}

int Radar::GetRange()
{
	return m_iRange;
}

void Radar::DrawContents()
{
	int size = Settings::Radar::size;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 center = {pos.x + m_flRadius, pos.y + m_flRadius};
	ImDrawList* draw = ImGui::GetWindowDrawList();

	draw->AddLine({ center.x, pos.y }, { center.x, pos.y + size }, IM_COL32(255,255,255,40));
	draw->AddLine({ pos.x, center.y }, { pos.x + size, center.y }, IM_COL32(255,255,255,40));

	if (EntityList::GetPlayerResources() == nullptr)
		return;

	if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
		return;

	CTFPlayer* pLocal = helper::engine::GetLocalPlayer();
	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	Vector localPos = pLocal->GetAbsOrigin();
	Vector viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	float viewYaw = viewAngles.y - 90.0f;

	int iconSize = Settings::Radar::icon_size;

	for (const auto& entry : EntityList::GetEntities())
	{
		if (entry.ptr == EntityList::GetLocal())
			continue;

		if (entry.flags & EntityFlags::IsBuilding && !Settings::Radar::buildings)
			continue;

		if (entry.flags & EntityFlags::IsPlayer && (!Settings::Radar::players || !static_cast<CTFPlayer*>(entry.ptr)->IsAlive()))
			continue;

		if (entry.flags & EntityFlags::IsProjectile && !Settings::Radar::projectiles)
			continue;

		Vec2 p = WorldToRadar(localPos, entry.ptr->GetAbsOrigin(), viewYaw);
		Color color = ESP::GetEntityColor(entry.ptr);

		draw->AddCircleFilled({ center.x + p.x, center.y - p.y }, iconSize, IM_COL32(color.r(), color.g(), color.b(), color.a()));

		/*if (entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsBuilding))
		{
			int health = 0;
			int maxhealth = 0;

			if (entry.flags & EntityFlags::IsPlayer)
			{
				CTFPlayer* pTarget = static_cast<CTFPlayer*>(entry.ptr);
				if (pTarget != nullptr)
				{
					health = pTarget->GetHealth();
					maxhealth = EntityList::GetPlayerResources()->m_iMaxHealth(pTarget->GetIndex());
				}
			}

			if (entry.flags & EntityFlags::IsBuilding)
			{
				CBaseObject* pTarget = static_cast<CBaseObject*>(entry.ptr);
				if (pTarget != nullptr)
				{
					health = pTarget->m_iHealth();
					maxhealth = pTarget->m_iMaxHealth();
				}
			}

			if (health != 0 && maxhealth != 0)
				DrawHealthbar(draw, ImVec2(center.x + p.x, center.y - p.y), health, maxhealth, iconSize);
		}*/
	}
}

void Radar::DrawHealthbar(ImDrawList* draw, ImVec2 pos, int health, int maxhealth, int iconSize)
{
	int half = static_cast<int>(iconSize);
	constexpr int barOffset = 3;

	draw->AddRectFilled(ImVec2(pos.x - half, pos.y + barOffset), ImVec2(pos.x + iconSize, pos.y + (barOffset*2.0f)), IM_COL32(255, 255, 255, 255));
}

void Radar::Init()
{
	m_iRange = 0;
	m_flRadius = 0;
}