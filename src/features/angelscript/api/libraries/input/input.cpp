#include "input.h"

#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

void Input_RegisterLibrary(asIScriptEngine *engine)
{
	auto defNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Input");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_INPUT);
		engine->RegisterGlobalFunction("bool IsButtonDown(int button)", asFUNCTION(Input_IsButtonDown),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsButtonPressed(int button)", asFUNCTION(Input_IsButtonPressed),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsButtonReleased(int button)", asFUNCTION(Input_IsButtonReleased),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorAlwaysVisible(bool state)",
					       asFUNCTION(Input_SetCursorAlwaysVisible), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsCursorVisible()", asFUNCTION(Input_IsCursorVisible),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetPollTick()", asFUNCTION(Input_GetPollTick), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsButtonValid()", asFUNCTION(Input_IsButtonValid), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(defNamespace);
}

bool Input_IsButtonDown(int button)
{
	ButtonCode_t btn = static_cast<ButtonCode_t>(button);
	if (!Input_IsButtonValid(button))
		return false;

	return interfaces::InputSystem->IsButtonDown(btn);
}

bool Input_IsButtonPressed(int button)
{
	ButtonCode_t btn = static_cast<ButtonCode_t>(button);
	if (!Input_IsButtonValid(button))
		return false;

	bool down = interfaces::InputSystem->IsButtonDown(btn);
	int tick  = interfaces::InputSystem->GetButtonPressedTick(btn);

	return down && interfaces::GlobalVars->tickcount >= tick;
}

bool Input_IsButtonReleased(int button)
{
	ButtonCode_t btn = static_cast<ButtonCode_t>(button);
	if (!Input_IsButtonValid(button))
		return false;

	bool down = interfaces::InputSystem->IsButtonDown(btn);
	int tick  = interfaces::InputSystem->GetButtonReleasedTick(btn);

	return down && interfaces::GlobalVars->tickcount >= tick;
}

void Input_SetCursorAlwaysVisible(bool state)
{
	interfaces::Surface->SetCursorAlwaysVisible(state);
}

bool Input_IsCursorVisible()
{
	return interfaces::Surface->IsCursorVisible();
}

int Input_GetPollTick()
{
	return interfaces::InputSystem->GetPollTick();
}

bool Input_IsButtonValid(int button)
{
	return button <= ButtonCode_t::BUTTON_CODE_INVALID || button >= ButtonCode_t::BUTTON_CODE_COUNT;
}