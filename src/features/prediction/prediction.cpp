#include "prediction.h"

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cgametrace.h"
#include "../../sdk/definitions/ctracefilters.h"
#include "../../sdk/helpers/engine/engine.h"
#include "../logs/logs.h"

#define COORD_INTEGER_BITS 14
#define COORD_FRACTIONAL_BITS 5
#define COORD_DENOMINATOR (1 << (COORD_FRACTIONAL_BITS))
#define COORD_RESOLUTION (1.0 / (COORD_DENOMINATOR))
#define MAX_CLIP_PLANES 5
#define DIST_EPSILON (0.03125)

CPrediction::CPrediction()
{
	m_pTarget	     = nullptr;

	m_vecAbsOrigin	     = {};
	m_vecVelocity	     = {};
	m_vecBaseVelocity    = {};
	m_vecMaxs	     = {};
	m_vecMins	     = {};
	m_vecWishDir	     = {};

	m_flGravity	     = 0.0f;
	m_flAccelerate	     = 0.0f;
	m_flStepSize	     = 0.0f;
	m_flFriction	     = 0.0f;
	m_flStopSpeed	     = 0.0f;
	m_flBounce	     = 0.0f;
	m_flTargetSeconds    = 0.0f;
	m_flMaxSpeed	     = 0.0f;

	m_bAllowAutoMovement = false;
	m_bIsOnGround	     = false;
	m_bIsStarted	     = false;
}

float CPrediction::GetGravity()
{
	return 800.0f * interfaces::GlobalVars->interval_per_tick * 0.5f;
}

void CPrediction::BeginPrediction(CTFPlayer *pEntity, float flTargetSeconds)
{
	if (m_bIsStarted)
		return Logs::Error("[CPrediction::BeginPrediction] Tried to start prediction "
				   "when it was already started!");

	if (!pEntity)
		return Logs::Error("[CPrediction::BeginPrediction] pEntity is nullptr!");

	static ConVar *sv_accelerate = interfaces::Cvar->FindVar("sv_accelerate");
	if (!sv_accelerate)
		return Logs::Error("[CPrediction::BeginPrediction] "
				   "sv_accelerate is nullptr!");

	static ConVar *sv_friction = interfaces::Cvar->FindVar("sv_friction");
	if (!sv_friction)
		return Logs::Error("[CPrediction::BeginPrediction] sv_friction is nullptr!");

	static ConVar *sv_stopspeed = interfaces::Cvar->FindVar("sv_stopspeed");
	if (!sv_stopspeed)
		return Logs::Error("[CPrediction::BeginPrediction] sv_stopspeed is nullptr!");

	static ConVar *sv_bounce = interfaces::Cvar->FindVar("sv_bounce");
	if (!sv_bounce)
		return Logs::Error("[CPrediction::BeginPrediction] sv_bounce is nullptr!");

	m_filter.pSkip = m_pTarget = pEntity;

	m_flAccelerate		   = sv_accelerate->GetFloat();
	m_flGravity		   = GetGravity();
	m_flFriction		   = sv_friction->GetFloat();
	m_flStopSpeed		   = sv_stopspeed->GetFloat();
	m_flStepSize		   = pEntity->m_flStepSize();

	m_bAllowAutoMovement	   = pEntity->m_bAllowAutoMovement();
	m_bIsOnGround		   = pEntity->GetFlags() & FL_ONGROUND;

	m_vecMaxs		   = pEntity->m_vecMaxs();
	m_vecMins		   = pEntity->m_vecMins();
	m_vecVelocity		   = pEntity->EstimateAbsVelocity();
	m_vecBaseVelocity	   = pEntity->m_vecBaseVelocity();
	m_vecAbsOrigin		   = m_bIsOnGround ? pEntity->m_vecOrigin() + Vec3{0, 0, 1} : pEntity->m_vecOrigin();

	m_flBounce		   = sv_bounce->GetFloat();
	m_flMaxSpeed		   = pEntity->m_flMaxspeed();
	m_flTargetSeconds	   = flTargetSeconds;
	m_flAirSpeedCap		   = GetAirSpeedCap();

	m_vecWishDir		   = m_vecVelocity;
	m_vecWishDir.z		   = 0;
	m_vecWishDir.Normalize();

	m_bIsStarted = true;
}

void CPrediction::EndPrediction()
{
	if (!m_bIsStarted)
		return Logs::Error("[CPrediction::EndPrediction] Tried to "
				   "end prediction while it wasn't started");

	m_pTarget = nullptr;

	m_vecAbsOrigin.Set();
	m_vecVelocity.Set();
	m_vecBaseVelocity.Set();
	m_vecMaxs.Set();
	m_vecMins.Set();
	m_vecWishDir.Set();

	m_flGravity	     = 0.0f;
	m_flAccelerate	     = 0.0f;
	m_flStepSize	     = 0.0f;
	m_flFriction	     = 0.0f;
	m_flStopSpeed	     = 0.0f;
	m_flBounce	     = 0.0f;
	m_flTargetSeconds    = 0.0f;
	m_flMaxSpeed	     = 0.0f;

	m_bAllowAutoMovement = false;
	m_bIsOnGround	     = false;
	m_bIsStarted	     = false;

	m_filter.pSkip	     = nullptr;
}

void CPrediction::BeginGravity()
{
	m_vecVelocity.z -= m_flGravity;
	m_vecVelocity.z += m_vecBaseVelocity.z * interfaces::GlobalVars->interval_per_tick;
}

void CPrediction::EndGravity()
{
	m_vecVelocity.z -= m_flGravity;
}

void CPrediction::Friction()
{
	float flSpeed = m_vecVelocity.Length();

	if (flSpeed < 0.1f)
		return;

	float flDrop	 = 0.0f;
	float flControl	 = 0.0f;
	float flFriction = 0.0f;

	if (IsOnGround())
	{
		flFriction = m_flFriction /* * player->surfaceFriction*/;
		flControl  = (flSpeed < m_flStopSpeed) ? m_flStopSpeed : flSpeed;
		flDrop += flControl * flFriction * interfaces::GlobalVars->interval_per_tick;
	}

	float flNewSpeed = flSpeed - flDrop;
	if (flNewSpeed < 0.0f)
		flNewSpeed = 0.0f;

	if (flNewSpeed != flSpeed)
	{
		flNewSpeed /= flSpeed;
		m_vecVelocity *= flNewSpeed;
	}
}

void CPrediction::TryTouchGround(const Vector &start, const Vector &end, const Vector &mins, const Vector &maxs,
				 unsigned int fMask, ITraceFilter &filter, trace_t &pm)
{
	Ray_t ray;
	ray.Init(start, end, mins, maxs);
	helper::engine::TraceHull(start, end, mins, maxs, fMask, &filter, &pm);
}

void CPrediction::TryTouchGroundInQuadrants(const Vector &start, const Vector &end, unsigned int fMask,
					    ITraceFilter &filter, CGameTrace &pm)
{
	Vector mins, maxs;
	Vector minsSrc = m_vecMins;
	Vector maxsSrc = m_vecMaxs;

	float fraction = pm.fraction;
	Vector endpos  = pm.endpos;

	// Check the -x, -y quadrant
	mins = minsSrc;
	maxs.Set(MIN(0, maxsSrc.x), MIN(0, maxsSrc.y), maxsSrc.z);
	TryTouchGround(start, end, mins, maxs, fMask, filter, pm);
	if (pm.m_pEnt && pm.plane.normal.z >= 0.7)
	{
		pm.fraction = fraction;
		pm.endpos   = endpos;
		return;
	}

	// Check the +x, +y quadrant
	mins.Set(MAX(0, minsSrc.x), MAX(0, minsSrc.y), minsSrc.z);
	maxs = maxsSrc;
	TryTouchGround(start, end, mins, maxs, fMask, filter, pm);
	if (pm.m_pEnt && pm.plane.normal.z >= 0.7)
	{
		pm.fraction = fraction;
		pm.endpos   = endpos;
		return;
	}

	// Check the -x, +y quadrant
	mins.Set(minsSrc.x, MAX(0, minsSrc.y), minsSrc.z);
	maxs.Set(MIN(0, maxsSrc.x), maxsSrc.y, maxsSrc.z);
	TryTouchGround(start, end, mins, maxs, fMask, filter, pm);
	if (pm.m_pEnt && pm.plane.normal.z >= 0.7)
	{
		pm.fraction = fraction;
		pm.endpos   = endpos;
		return;
	}

	// Check the +x, -y quadrant
	mins.Set(MAX(0, minsSrc.x), minsSrc.y, minsSrc.z);
	maxs.Set(maxsSrc.x, MIN(0, maxsSrc.y), maxsSrc.z);
	TryTouchGround(start, end, mins, maxs, fMask, filter, pm);
	if (pm.m_pEnt && pm.plane.normal.z >= 0.7)
	{
		pm.fraction = fraction;
		pm.endpos   = endpos;
		return;
	}

	pm.fraction = fraction;
	pm.endpos   = endpos;
}

bool CPrediction::IsOnGround()
{
	CGameTrace trace;

	Vec3 vecStart, vecEnd;
	vecEnd	 = m_vecAbsOrigin;
	vecStart = vecEnd - Vec3{0, 0, 2.0f};

	TryTouchGround(vecStart, vecEnd, m_vecMins, m_vecMaxs, MASK_PLAYERSOLID, m_filter, trace);

	if (trace.fraction < 1.0f && trace.plane.normal.z >= 0.7f)
		return true;

	TryTouchGroundInQuadrants(vecStart, vecEnd, MASK_PLAYERSOLID, m_filter, trace);

	return trace.fraction < 1.0f && trace.plane.normal.z >= 0.7f;
}

void CPrediction::StayOnGround()
{
	CGameTrace trace;
	CTraceFilterWorldAndPropsOnly filter;
	filter.pSkip = m_pTarget;

	Vec3 vecStart{m_vecAbsOrigin};
	Vec3 vecEnd{m_vecAbsOrigin};

	vecStart.z -= 2;
	vecEnd.z -= m_flStepSize;

	helper::engine::TraceHull(m_vecAbsOrigin, vecStart, m_vecMins, m_vecMaxs, MASK_PLAYERSOLID, &filter, &trace);

	if (trace.fraction > 0.0f &&	 // must go somewhere
	    trace.fraction < 1.0f &&	 // must hit something
	    !trace.startsolid &&	 // can't be embedded in a solid
	    trace.plane.normal.z >= 0.7) // can't hit a steep slope that we
					 // can't stand on anyway
	{
		float flDelta = fabs(m_vecAbsOrigin.z - trace.endpos.z);

		// This is incredibly hacky. The real problem is that trace
		// returning that strange value we can't network over.
		if (flDelta > 0.5f * COORD_RESOLUTION)
			m_vecAbsOrigin = trace.endpos;
	}
}

int CPrediction::ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce,
			      float flRedirectCoeff /* = 0.f */)
{
	float backoff;
	float change;
	float angle;
	int i, blocked;

	angle	= normal[2];

	blocked = 0x00;		 // Assume unblocked.
	if (angle > 0)		 // If the plane that is blocking us has a positive z
				 // component, then assume it's a floor.
		blocked |= 0x01; //
	if (!angle)		 // If the plane has no Z, it is vertical (wall/step)
		blocked |= 0x02; //

	// Determine how far along plane to slide based on incoming direction.
	float flBlocked = in.Dot(normal);
	backoff		= flBlocked * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
	}

	// iterate once to make sure we aren't still moving through the plane
	float adjust = out.Dot(normal);
	if (adjust < 0.0f)
	{
		out -= (normal * adjust);
		//		Msg( "Adjustment = %lf\n", adjust );
	}

	if (flRedirectCoeff > 0.f)
	{
		// Redirect clipped velocity along angle of movement
		float flLen = out.Length();
		out *= (-1.f * flBlocked * flRedirectCoeff + flLen) / flLen;
	}

	// Return blocking flags.
	return blocked;
}

int CPrediction::TryPlayerMove(Vector *pFirstDest, CGameTrace *pFirstTrace, float flSlideMultiplier /* = 0.f */)
{
	int bumpcount, numbumps;
	Vector dir;
	float d;
	int numplanes;
	Vector planes[MAX_CLIP_PLANES];
	Vector primal_velocity, original_velocity;
	Vector new_velocity;
	int i, j;
	trace_t pm;
	Vector end;
	float time_left, allFraction;
	int blocked;

	numbumps	  = 4; // Bump up to four times

	blocked		  = 0; // Assume not blocked
	numplanes	  = 0; //  and not sliding along any planes

	original_velocity = m_vecVelocity;
	primal_velocity	  = m_vecVelocity;

	allFraction	  = 0;
	time_left	  = interfaces::GlobalVars->interval_per_tick; // Total time for this
								       // movement operation.

	new_velocity.Set();

	for (bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		if (m_vecVelocity.Length() == 0.0)
			break;

		// Assume we can move all the way from the current origin to
		// the
		//  end point.
		end = m_vecAbsOrigin + (m_vecVelocity * time_left);

		// See if we can make it from origin to end point.
		// If their velocity Z is 0, then we can avoid an extra trace
		// here during WalkMove.
		if (pFirstDest && end == *pFirstDest)
			pm = *pFirstTrace;
		else
			TracePlayerBBox(m_vecAbsOrigin, end, MASK_PLAYERSOLID, m_filter, pm);

		allFraction += pm.fraction;

		// If we started in a solid object, or we were in solid space
		//  the whole way, zero out our velocity and return that we
		//  are blocked by floor and wall.
		if (pm.allsolid)
		{
			// entity is trapped in another solid
			m_vecVelocity.Set();
			return 4;
		}

		// If we moved some portion of the total distance, then
		//  copy the end position into the pmove.origin and
		//  zero the plane counter.
		if (pm.fraction > 0)
		{
			if (numbumps > 0 && pm.fraction == 1)
			{
				// There's a precision issue with terrain
				// tracing that can cause a swept box to
				// successfully trace when the end position is
				// stuck in the triangle.  Re-run the test with
				// an uswept box to catch that case until the
				// bug is fixed. If we detect getting stuck,
				// don't allow the movement
				trace_t stuck;
				TracePlayerBBox(pm.endpos, pm.endpos, MASK_PLAYERSOLID, m_filter, stuck);
				if (stuck.startsolid || stuck.fraction != 1.0f)
				{
					// Msg( "Player will become stuck!!!\n"
					// );
					m_vecVelocity.Set();
					break;
				}
			}

			// actually covered some distance
			m_vecAbsOrigin	  = pm.endpos;
			original_velocity = m_vecVelocity;
			numplanes	  = 0;
		}

		// If we covered the entire distance, we are done
		//  and can return.
		if (pm.fraction == 1)
			break; // moved the entire distance

		// If the plane we hit has a high z component in the normal,
		// then
		//  it's probably a floor
		if (pm.plane.normal.z > 0.7)
		{
			blocked |= 1; // floor
		}
		// If the plane has a zero z component in the normal, then it's
		// a
		//  step or wall
		if (!pm.plane.normal.z)
		{
			blocked |= 2; // step / wall
		}

		// Reduce amount of m_flFrameTime left by total time left *
		// fraction
		//  that we covered.
		time_left -= time_left * pm.fraction;

		// Did we run out of planes to clip against?
		if (numplanes >= MAX_CLIP_PLANES)
		{
			// this shouldn't really happen
			//  Stop our movement if so.
			m_vecVelocity.Set();
			// Con_DPrintf("Too many planes 4\n");

			break;
		}

		// Set up next clipping plane
		planes[numplanes] = pm.plane.normal;
		numplanes++;

		// modify original_velocity so it parallels all of the clip
		// planes
		//

		// reflect player velocity
		// Only give this a try for first impact plane because you can
		// get yourself stuck in an acute corner by jumping in place
		//  and pressing forward and nobody was really using this
		//  bounce/reflection feature anyway...
		if (numplanes == 1 && m_bIsOnGround)
		{
			for (i = 0; i < numplanes; i++)
			{
				if (planes[i].z > 0.7)
				{
					// floor or slope
					ClipVelocity(original_velocity, planes[i], new_velocity, 1, flSlideMultiplier);
					original_velocity = new_velocity;
				}
				else
				{
					ClipVelocity(original_velocity, planes[i], new_velocity,
						     1.0 + m_flBounce * (1 - 1 /*player->m_surfaceFriction*/),
						     flSlideMultiplier);
				}
			}

			m_vecVelocity	  = new_velocity;
			original_velocity = new_velocity;
		}
		else
		{
			for (i = 0; i < numplanes; i++)
			{
				ClipVelocity(original_velocity, planes[i], m_vecVelocity, 1, flSlideMultiplier);

				for (j = 0; j < numplanes; j++)
					if (j != i)
					{
						// Are we now moving against
						// this plane?
						if (m_vecVelocity.Dot(planes[j]) < 0)
							break; // not ok
					}
				if (j == numplanes) // Didn't have to clip, so
						    // we're ok
					break;
			}

			// Did we go all the way through plane set
			if (i != numplanes)
			{ // go along this plane
				// pmove.velocity is set in clipping call, no
				// need to set again.
				;
			}
			else
			{ // go along the crease
				if (numplanes != 2)
				{
					m_vecVelocity.Set();
					break;
				}
				dir = planes[0].Dot(planes[1]);
				dir.Normalize();
				d = dir.Dot(m_vecVelocity);
				m_vecVelocity *= dir * d;
			}

			//
			// if original velocity is against the original
			// velocity, stop dead to avoid tiny occilations in
			// sloping corners
			//
			d = m_vecVelocity.Dot(primal_velocity);
			if (d <= 0)
			{
				// Con_DPrintf("Back\n");
				m_vecVelocity.Set();
				break;
			}
		}
	}

	if (allFraction == 0)
		m_vecVelocity.Set();

	return blocked;
}

void CPrediction::StepMove(Vector &vecDestination, CGameTrace &trace)
{
	Vector vecEndPos;
	vecEndPos = vecDestination;

	// Try sliding forward both on ground and up 16 pixels
	//  take the move that goes farthest
	Vector vecPos, vecVel;
	vecPos = m_vecAbsOrigin;
	vecVel = m_vecVelocity;

	// Slide move down.
	TryPlayerMove(&vecEndPos, &trace);

	// Down results.
	Vector vecDownPos, vecDownVel;
	vecDownPos = m_vecAbsOrigin;
	vecDownVel = m_vecVelocity;

	// Reset original values.
	m_vecAbsOrigin = vecPos;
	m_vecVelocity  = vecVel;

	// Move up a stair height.
	vecEndPos = m_vecAbsOrigin;
	if (m_bAllowAutoMovement)
		vecEndPos.z += m_flStepSize + DIST_EPSILON;

	TracePlayerBBox(m_vecAbsOrigin, vecEndPos, MASK_PLAYERSOLID, m_filter, trace);
	if (!trace.startsolid && !trace.allsolid)
		SetAbsOrigin(trace.endpos);

	// Slide move up.
	TryPlayerMove();

	// Move down a stair (attempt to).
	vecEndPos = m_vecAbsOrigin;
	if (m_bAllowAutoMovement)
	{
		vecEndPos.z -= m_flStepSize + DIST_EPSILON;
	}

	TracePlayerBBox(m_vecAbsOrigin, vecEndPos, MASK_PLAYERSOLID, m_filter, trace);

	// If we are not on the ground any more then use the original movement
	// attempt.
	if (trace.plane.normal.z < 0.7)
	{
		SetAbsOrigin(vecDownPos);
		m_vecVelocity = vecDownVel;
		return;
	}

	// If the trace ended up in empty space, copy the end over to the
	// origin.
	if (!trace.startsolid && !trace.allsolid)
	{
		SetAbsOrigin(trace.endpos);
	}

	// Copy this origin to up.
	Vector vecUpPos;
	vecUpPos = m_vecAbsOrigin;

	// decide which one went farther
	float flDownDist = (vecDownPos.x - vecPos.x) * (vecDownPos.x - vecPos.x) +
			   (vecDownPos.y - vecPos.y) * (vecDownPos.y - vecPos.y);
	float flUpDist =
	    (vecUpPos.x - vecPos.x) * (vecUpPos.x - vecPos.x) + (vecUpPos.y - vecPos.y) * (vecUpPos.y - vecPos.y);
	if (flDownDist > flUpDist)
	{
		SetAbsOrigin(vecDownPos);
		m_vecVelocity = vecDownVel;
	}
	else
	{
		// copy z value from slide move
		m_vecVelocity.z = vecDownVel.z;
	}
}

void CPrediction::TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, ITraceFilter &filter,
				  CGameTrace &trace)
{
	helper::engine::TraceHull(start, end, m_vecMins, m_vecMaxs, fMask, &filter, &trace);
}

void CPrediction::SetAbsOrigin(const Vector &in)
{
	m_vecAbsOrigin = in;
}

bool CPrediction::CheckWater(void)
{
	Vector point;
	int cont;

	Vector vPlayerMins = m_vecMins;
	Vector vPlayerMaxs = m_vecMaxs;

	int level	   = 0;

	// Pick a spot just above the players feet.
	point.x = m_vecAbsOrigin.x + (vPlayerMins.x + vPlayerMaxs.x) * 0.5;
	point.y = m_vecAbsOrigin.y + (vPlayerMins.y + vPlayerMaxs.y) * 0.5;
	point.z = m_vecAbsOrigin.z + vPlayerMins.z + 1;

	// Assume that we are not in water at all.
	level = WL_NotInWater;

	// Grab point contents.
	cont = interfaces::EngineTrace->GetPointContents(point);

	// Are we under water? (not solid and not empty?)
	if (cont & MASK_WATER)
	{
		// We are at least at level one
		level = WL_Feet;

		// Now check a point that is at the player hull midpoint.
		point.z = GetAbsOrigin().z + (vPlayerMins.z + vPlayerMaxs.z) * 0.5;
		cont	= interfaces::EngineTrace->GetPointContents(point);
		// If that point is also under water...
		if (cont & MASK_WATER)
		{
			// Set a higher water level.
			level = WL_Waist;

			// Now check the eye position.  (view_ofs is relative
			// to the origin)
			point.z = GetAbsOrigin().z + m_pTarget->m_vecViewOffset().z;
			cont	= interfaces::EngineTrace->GetPointContents(point);
			if (cont & MASK_WATER)
				level = WL_Eyes; // In over our eyes
		}
	}

	return level > WL_Feet;
}

void CPrediction::Accelerate(Vector &wishdir, float wishspeed, float accel)
{
	int i;
	float addspeed, accelspeed, currentspeed;

	// See if we are changing direction a bit
	currentspeed = m_vecVelocity.Dot(wishdir);

	// Reduce wishspeed by the amount of veer.
	addspeed = wishspeed - currentspeed;

	// If not going to add any speed, done.
	if (addspeed <= 0)
		return;

	// Determine amount of accleration.
	accelspeed = accel * interfaces::GlobalVars->interval_per_tick * wishspeed /* * player->m_surfaceFriction*/;

	// Cap at addspeed
	if (accelspeed > addspeed)
		accelspeed = addspeed;

	// Adjust velocity.
	m_vecVelocity += wishdir * accelspeed;
}

void CPrediction::WalkMove(void)
{
	Vector wishdir	= m_vecWishDir;
	float wishspeed = m_flMaxSpeed;

	Vector dest;
	CGameTrace pm;
	bool oldground = m_bIsOnGround;

	// Clamp to server defined max speed (wishspeed already is, but guard
	// anyway)
	float spd = wishdir.Length();
	if (spd > 0.0f && spd > m_flMaxSpeed)
	{
		wishdir *= m_flMaxSpeed / spd;
		wishspeed = m_flMaxSpeed;
	}

	m_vecVelocity.z = 0;
	Accelerate(wishdir, wishspeed, m_flAccelerate);
	m_vecVelocity.z = 0;

	m_vecVelocity += m_vecBaseVelocity;

	spd = m_vecVelocity.Length();
	if (spd < 1.0f)
	{
		m_vecVelocity.Set();
		m_vecVelocity -= m_vecBaseVelocity;
		return;
	}

	dest.x = GetAbsOrigin().x + m_vecVelocity.x * interfaces::GlobalVars->interval_per_tick;
	dest.y = GetAbsOrigin().y + m_vecVelocity.y * interfaces::GlobalVars->interval_per_tick;
	dest.z = GetAbsOrigin().z;

	TracePlayerBBox(GetAbsOrigin(), dest, MASK_PLAYERSOLID, m_filter, pm);

	if (pm.fraction == 1)
	{
		SetAbsOrigin(pm.endpos);
		m_vecVelocity -= m_vecBaseVelocity;
		StayOnGround();
		return;
	}

	if (oldground == false && CheckWater() == 0)
	{
		m_vecVelocity -= m_vecBaseVelocity;
		return;
	}

	StepMove(dest, pm);
	m_vecVelocity -= m_vecBaseVelocity;
	StayOnGround();
}

void CPrediction::AirAccelerate(Vector &wishdir, float wishspeed, float accel)
{
	int i;
	float addspeed, accelspeed, currentspeed;
	float wishspd;

	wishspd = wishspeed;

	// Cap speed
	if (wishspd > m_flAirSpeedCap)
		wishspd = m_flAirSpeedCap;

	// Determine veer amount
	currentspeed = m_vecVelocity.Dot(wishdir);

	// See how much to add
	addspeed = wishspd - currentspeed;

	// If not adding any, done.
	if (addspeed <= 0)
		return;

	// Determine acceleration speed after acceleration
	accelspeed = accel * wishspeed * interfaces::GlobalVars->interval_per_tick /* * player->m_surfaceFriction*/;

	// Cap it
	if (accelspeed > addspeed)
		accelspeed = addspeed;

	// Adjust pmove vel.
	m_vecVelocity += wishdir * accelspeed;
}

void CPrediction::AirMove(void)
{
	Vector wishdir	= m_vecWishDir;
	float wishspeed = wishdir.Length();

	if (wishspeed != 0.0f && wishspeed > m_flMaxSpeed)
	{
		wishdir *= m_flMaxSpeed / wishspeed;
		wishspeed = m_flMaxSpeed;
	}

	AirAccelerate(wishdir, wishspeed, m_flAccelerate);

	m_vecVelocity += m_vecBaseVelocity;
	TryPlayerMove();
	m_vecVelocity -= m_vecBaseVelocity;
}

bool CPrediction::Simulate(std::vector<Vector> &path)
{
	if (!m_bIsStarted)
	{
		Logs::Error("[CPrediction::Simulate] Tried simulating while "
			    "prediction not started!");
		return false;
	}

	float flClock = 0.0f;

	while (flClock < m_flTargetSeconds)
	{
		if (!CheckWater())
			BeginGravity();

		m_bIsOnGround = IsOnGround();

		if (m_bIsOnGround)
		{
			m_vecVelocity.z = 0.0f;
			Friction();
		}

		if (m_bIsOnGround)
			WalkMove();
		else
			AirMove();

		if (!CheckWater())
			EndGravity();

		if (m_bIsOnGround)
			m_vecVelocity.z = 0.0f;

		path.emplace_back(m_vecAbsOrigin);
		flClock += interfaces::GlobalVars->interval_per_tick;
	}

	return true;
}

Vector &CPrediction::GetAbsOrigin()
{
	return m_vecAbsOrigin;
}

float CPrediction::GetAirSpeedCap()
{
	if (m_pTarget->InCond(TF_COND_SHIELD_CHARGE))
	{
		static ConVar *tf_max_charge_speed = interfaces::Cvar->FindVar("tf_max_charge_speed");
		return tf_max_charge_speed->GetFloat();
	}

	float flCap = 30.0f;

	if (m_pTarget->InCond(TF_COND_PARACHUTE_DEPLOYED))
	{
		static ConVar *tf_parachute_aircontrol = interfaces::Cvar->FindVar("tf_parachute_aircontrol");
		flCap *= tf_parachute_aircontrol->GetFloat();
	}

	float mod_air_control = AttributeHookValue(1.0f, "mod_air_control", m_pTarget, nullptr, true);
	return flCap * mod_air_control;
}

CPrediction gPrediction{};