#pragma once

#include "cbaseobject.h"

class CObjectDispenser: public CBaseObject
{
public:
	NETVAR(m_iState, "CObjectDispenser->m_iState", int);
	NETVAR(m_iAmmoMetal, "CObjectDispenser->m_iAmmoMetal", int);
	NETVAR(m_iMiniBombCounter, "CObjectDispenser->m_iMiniBombCounter", int);
};