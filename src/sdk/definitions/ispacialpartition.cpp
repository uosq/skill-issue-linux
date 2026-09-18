#include "ispacialpartition.h"
#include "../interfaces/interfaces.h"
#include "../classes/entity.h"

IterationRetval_t CFlaggedEntitiesEnum::EnumElement(IHandleEntity* entity)
{
	IClientEntity *pClientEntity = interfaces::EntityList->GetClientEntityFromHandle( entity->GetRefEHandle() );
	CBaseEntity *pEntity = pClientEntity ? pClientEntity->GetBaseEntity() : nullptr;
	if ( pEntity )
	{
		if ( m_flagMask && !(pEntity->m_fFlags() & m_flagMask) )	// Does it meet the criteria?
			return ITERATION_CONTINUE;

		if ( !AddToList( pEntity ) )
			return ITERATION_STOP;
	}

	return ITERATION_CONTINUE;
}
