#pragma once

#include "definitions/cbasehandle.h"
#include "definitions/ihandleentity.h"
#include "interfaces/interfaces.h"

/*
I would like to say how much I hate c++
but then I would waste a month talking non stop about the fucking header system
fuck you whoever thought it was a good idea
*/

inline IHandleEntity* CBaseHandle::Get() const
{
    return reinterpret_cast<IHandleEntity*>(interfaces::EntityList->GetClientEntity(m_Index));
}

//Casts to T
template <typename T>
inline T* HandleAs(const CBaseHandle& h)
{
    return reinterpret_cast<T*>(interfaces::EntityList->GetClientEntity(h.GetEntryIndex()));
}
