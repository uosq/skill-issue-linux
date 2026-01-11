#pragma once

#include "../../sdk/netvars/netvar.h"

class CTFPlayerResource
{
public:
	NETVAR(m_iTotalScore, "CTFPlayerResource->m_iTotalScore", int*);
	NETVAR(m_iMaxHealth, "CTFPlayerResource->m_iMaxHealth", int*);
	NETVAR(m_iMaxBuffedHealth, "CTFPlayerResource->m_iMaxBuffedHealth", int*);
	NETVAR(m_iPlayerClass, "CTFPlayerResource->m_iPlayerClass", int*);
	NETVAR(m_bArenaSpectator, "CTFPlayerResource->m_bArenaSpectator", bool*);
	NETVAR(m_iActiveDominations, "CTFPlayerResource->m_iActiveDominations", int*);
	NETVAR(m_flNextRespawnTime, "CTFPlayerResource->m_flNextRespawnTime", float*);
	NETVAR(m_iChargeLevel, "CTFPlayerResource->m_iChargeLevel", int*);
	NETVAR(m_iDamage, "CTFPlayerResource->m_iDamage", int*);
	NETVAR(m_iDamageAssist, "CTFPlayerResource->m_iDamageAssist", int*);
	NETVAR(m_iDamageBoss, "CTFPlayerResource->m_iDamageBoss", int*);
	NETVAR(m_iHealing, "CTFPlayerResource->m_iHealing", int*);
	NETVAR(m_iHealingAssist, "CTFPlayerResource->m_iHealingAssist", int*);
	NETVAR(m_iDamageBlocked, "CTFPlayerResource->m_iDamageBlocked", int*);
	NETVAR(m_iCurrencyCollected, "CTFPlayerResource->m_iCurrencyCollected", int*);
	NETVAR(m_iBonusPoints, "CTFPlayerResource->m_iBonusPoints", int*);
	NETVAR(m_iPlayerLevel, "CTFPlayerResource->m_iPlayerLevel", int*);
	NETVAR(m_iStreaks, "CTFPlayerResource->m_iStreaks", int*);
	NETVAR(m_iUpgradeRefundCredits, "CTFPlayerResource->m_iUpgradeRefundCredits", int*);
	NETVAR(m_iBuybackCredits, "CTFPlayerResource->m_iBuybackCredits", int*);
	NETVAR(m_iPartyLeaderRedTeamIndex, "CTFPlayerResource->m_iPartyLeaderRedTeamIndex", int*);
	NETVAR(m_iPartyLeaderBlueTeamIndex, "CTFPlayerResource->m_iPartyLeaderBlueTeamIndex", int*);
	NETVAR(m_iEventTeamStatus, "CTFPlayerResource->m_iEventTeamStatus", int*);
	NETVAR(m_iPlayerClassWhenKilled, "CTFPlayerResource->m_iPlayerClassWhenKilled", int*);
	NETVAR(m_iConnectionState, "CTFPlayerResource->m_iConnectionState", int*);
	NETVAR(m_flConnectTime, "CTFPlayerResource->m_flConnectTime", float*);
};