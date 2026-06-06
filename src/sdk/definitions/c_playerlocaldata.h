#pragma once

#include "ivrenderview.h"
#include "types.h"
#include "playernet_vars.h"

class CPlayerLocalData
{
public:
	virtual void NetworkStateChanged() {}  virtual void NetworkStateChanged( void *pProp ) {}

	CPlayerLocalData()
	{
		m_flFOVRate = 0;
	}

	unsigned char			m_chAreaBits[MAX_AREA_STATE_BYTES];				// Area visibility flags.
	unsigned char			m_chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES];// Area portal visibility flags.

	int						m_iHideHUD;			// bitfields containing sections of the HUD to hide
	
	float					m_flFOVRate;		// rate at which the FOV changes
	

	bool					m_bDucked;
	bool					m_bDucking;
	bool					m_bInDuckJump;
	float					m_flDucktime;
	float					m_flDuckJumpTime;
	float					m_flJumpTime;
	int						m_nStepside;
	float					m_flFallVelocity;
	int						m_nOldButtons;
	float					m_flOldForwardMove;
	// Base velocity that was passed in to server physics so 
	//  client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
	Vec3 m_vecClientBaseVelocity;  
	Vec3 m_vecPunchAngle;	// auto-decaying view angle adjustment
	Vec3 m_iv_vecPunchAngle;

	Vec3 m_vecPunchAngleVel;		// velocity of auto-decaying view angle adjustment
	Vec3 m_iv_vecPunchAngleVel;
	bool					m_bDrawViewmodel;
	bool					m_bWearingSuit;
	bool					m_bPoisoned;
	float					m_flStepSize;
	bool					m_bAllowAutoMovement;

	// 3d skybox
	sky3dparams_t			m_skybox3d;
	// fog params
	fogplayerparams_t		m_PlayerFog;
	// audio environment
	audioparams_t			m_audio;

	bool					m_bSlowMovement;

};