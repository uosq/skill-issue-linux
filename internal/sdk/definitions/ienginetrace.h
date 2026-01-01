#pragma once

#include "../math.h"
#include "cmodel.h"

class IHandleEntity;
class CGameTrace;
typedef CGameTrace trace_t;
class ICollideable;
class CTraceListData;
class CPhysCollide;
struct cplane_t;

class CGameTrace;
typedef CGameTrace trace_t;
class ICollideable;
class CTraceListData;
class CPhysCollide;
struct cplane_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int		GetPointContents( const Vector &vecAbsPosition, IHandleEntity** ppEntity = NULL ) = 0;
	
	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual int		GetPointContents_Collideable( ICollideable *pCollide, const Vector &vecAbsPosition ) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToEntity( const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace ) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToCollideable( const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, trace_t *pTrace ) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void	TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace ) = 0;

	// A version that sets up the leaf and entity lists and allows you to pass those in for collision.
	virtual void	SetupLeafAndEntityListRay( const Ray_t &ray, CTraceListData &traceData ) = 0;
	virtual void    SetupLeafAndEntityListBox( const Vector &vecBoxMin, const Vector &vecBoxMax, CTraceListData &traceData ) = 0;
	virtual void	TraceRayAgainstLeafAndEntityList( const Ray_t &ray, CTraceListData &traceData, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace ) = 0;

	// A version that sweeps a collideable through the world
	// abs start + abs end represents the collision origins you want to sweep the collideable through
	// vecAngles represents the collision angles of the collideable during the sweep
	virtual void	SweepCollideable( ICollideable *pCollide, const Vector &vecAbsStart, const Vector &vecAbsEnd, 
		const QAngle &vecAngles, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace ) = 0;

	// Enumerates over all entities along a ray
	// If triggers == true, it enumerates all triggers along a ray
	virtual void	EnumerateEntities( const Ray_t &ray, bool triggers, IEntityEnumerator *pEnumerator ) = 0;

	// Same thing, but enumerate entitys within a box
	virtual void	EnumerateEntities( const Vector &vecAbsMins, const Vector &vecAbsMaxs, IEntityEnumerator *pEnumerator ) = 0;

	// Convert a handle entity to a collideable.  Useful inside enumer
	virtual ICollideable *GetCollideable( IHandleEntity *pEntity ) = 0;

	// HACKHACK: Temp for performance measurments
	virtual int GetStatByIndex( int index, bool bClear ) = 0;

	//finds brushes in an AABB, prone to some false positives
	virtual void GetBrushesInAABB( const Vector &vMins, const Vector &vMaxs, void *pOutput, int iContentsMask = 0xFFFFFFFF ) = 0;

	//Creates a CPhysCollide out of all displacements wholly or partially contained in the specified AABB
	virtual CPhysCollide* GetCollidableFromDisplacementsInAABB( const Vector& vMins, const Vector& vMaxs ) = 0;

	//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
	virtual bool GetBrushInfo( int iBrush, void *pPlanesOut, int *pContentsOut ) = 0;

	virtual bool PointOutsideWorld( const Vector &ptTest ) = 0; //Tests a point to see if it's outside any playable area

	// Walks bsp to find the leaf containing the specified point
	virtual int GetLeafContainingPoint( const Vector &ptTest ) = 0;
};
