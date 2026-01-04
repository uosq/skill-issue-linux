#pragma once

#include "cbaseobject.h"

class CObjectTeleporter: public CBaseObject
{
public:
	NETVAR(m_iState, "CObjectTeleporter->m_iState", int);
	NETVAR(m_flRechargeTime, "CObjectTeleporter->m_flRechargeTime", float);
	NETVAR(m_flCurrentRechargeDuration, "CObjectTeleporter->m_flCurrentRechargeDuration", float);
	NETVAR(m_iTimesUsed, "CObjectTeleporter->m_iTimesUsed", int);
	NETVAR(m_flYawToExit, "CObjectTeleporter->m_flYawToExit", float);
	NETVAR(m_bMatchBuilding, "CObjectTeleporter->m_bMatchBuilding", bool);
};