#pragma once

#include "../../../include/angelscript.h"

void Input_RegisterLibrary(asIScriptEngine *engine);
bool Input_IsButtonDown(int button);
bool Input_IsButtonPressed(int button);
bool Input_IsButtonReleased(int button);
void Input_SetCursorAlwaysVisible(bool state);
bool Input_IsCursorVisible();
int Input_GetPollTick();
bool Input_IsButtonValid(int button);