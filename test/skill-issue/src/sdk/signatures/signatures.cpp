#include "signatures.h"
#include "../interfaces/interfaces.h"

bool Sigs_InitAll()
{
	bool result = true;

	for (Sig *sig : Sig::GetRegistry())
	{
		if (!sig || !sig->Init())
		{
			result = false;
			interfaces::Cvar->ConsolePrintf("Error: signature %s is null!", sig->GetName().c_str());
			break;
		}
	}

	return result;
}