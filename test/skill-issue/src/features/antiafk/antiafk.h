#pragma once

#include "../feature.h"

class CUserCmd;

class Antiafk
{
public:
	void OnCreateMove(CUserCmd* pCmd);
	void OnLevelShutdown();

private:
	int m_afkticks;
};

DECLARE_FEATURE(Antiafk, antiafk)