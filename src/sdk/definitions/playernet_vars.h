#pragma once

#include <array>

#include "cbasehandle.h"
#include "color.h"
#include "types.h"

#define NUM_AUDIO_LOCAL_SOUNDS	8

class CBaseEntity;
class CFogController;

struct fogparams_t
{
	bool operator !=( const fogparams_t& other ) const;

	Vec3 dirPrimary;
	Color colorPrimary;
	Color colorSecondary;
	Color colorPrimaryLerpTo;
	Color colorSecondaryLerpTo;
	float start;
	float end;
	float farz;
	float maxdensity;

	float startLerpTo;
	float endLerpTo;
	float lerptime;
	float duration;
	bool enable;
	bool blend;
};

class CFogController;

struct fogplayerparams_t
{
	CHandle<CFogController> m_hCtrl;
	float					m_flTransitionTime;

	Color					m_OldColor;
	float					m_flOldStart;
	float					m_flOldEnd;

	Color					m_NewColor;
	float					m_flNewStart;
	float					m_flNewEnd;

	fogplayerparams_t()
	{
		m_hCtrl.Set( NULL );
		m_flTransitionTime = -1.0f;
		m_OldColor.SetColor(0, 0, 0, 0);
		m_flOldStart = 0.0f;
		m_flOldEnd = 0.0f;
		m_NewColor.SetColor(0, 0, 0, 0);
		m_flNewStart = 0.0f;
		m_flNewEnd = 0.0f;
	}
};

struct sky3dparams_t
{
	// 3d skybox camera data
	int scale;
	Vec3 origin;
	int area;

	// 3d skybox fog data
	fogparams_t fog;
};

struct audioparams_t
{
	std::array<Vector, NUM_AUDIO_LOCAL_SOUNDS> localSound;
	int soundscapeIndex;	// index of the current soundscape from soundscape.txt
	int localBits;			// if bits 0,1,2,3 are set then position 0,1,2,3 are valid/used
	CHandle<CBaseEntity> ent;		// the entity setting the soundscape
};