#pragma once

#include "funcs.h"

namespace Lua
{
	extern lua_State* m_luaState;

	void InitPluto();
	void ClosePluto();
	void RunCode(std::string text);
}