#pragma once

#include "../feature.h"
#include "../config/config.h"

BEGIN_CONFIG(antiafk)
	ADD_CONFIG(enabled, "antiafk enabled", false)
END_CONFIG()

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