#include "../sdk/interfaces/interfaces.h"

#include "../features/hookmanager/hookmanager.hpp"

using GetUserCmdFn = CUserCmd *(*)(void *input, int sequence_number);

INIT_VTABLE_HOOK(GetUserCmd, CUserCmd*, (void* input, int sequence), interfaces::CInput, 8)
{
	return &(static_cast<IInput *>(interfaces::CInput)->m_pCommands[sequence % 90]);
}