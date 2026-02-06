#pragma once

#include "libraries.h"

namespace Lua
{
	extern lua_State* m_luaState;

	void InitPluto();
	void ClosePluto();
	void RunCode(std::string text);
}