#pragma once

#include "cbaseobject.h"

class CObjectSentrygun: public CBaseObject
{
public:
	NETVAR(m_iAmmoShells, "CObjectSentrygun->m_iAmmoShells", int);
	NETVAR(m_iAmmoRockets, "CObjectSentrygun->m_iAmmoRockets", int);
	NETVAR(m_iState, "CObjectSentrygun->m_iState", int);
	NETVAR(m_bPlayerControlled, "CObjectSentrygun->m_bPlayerControlled", bool);
	NETVAR(m_nShieldLevel, "CObjectSentrygun->m_nShieldLevel", int);
	NETVAR(m_bShielded, "CObjectSentrygun->m_bShielded", bool);
	NETVAR(m_hEnemy, "CObjectSentrygun->m_hEnemy", EHANDLE);
	NETVAR(m_hAutoAimTarget, "CObjectSentrygun->m_hAutoAimTarget", EHANDLE);
	NETVAR(m_iKills, "CObjectSentrygun->m_iKills", int);
	NETVAR(m_iAssists, "CObjectSentrygun->m_iAssists", int);
};