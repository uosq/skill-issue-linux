#pragma once

class CUserCmd;

namespace AntiAFK
{
	void OnCreateMove(CUserCmd* pCmd);
	void OnLevelShutdown();
}