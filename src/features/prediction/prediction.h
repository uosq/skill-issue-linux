#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/definitions/cgametrace.h"
#include "../../sdk/definitions/ctracefilters.h"
#include "../../sdk/definitions/bspflags.h"
#include <algorithm>
#include <vector>

#define	COORD_INTEGER_BITS			14
#define COORD_FRACTIONAL_BITS		5
#define COORD_DENOMINATOR			(1<<(COORD_FRACTIONAL_BITS))
#define COORD_RESOLUTION			(1.0/(COORD_DENOMINATOR))

namespace PlayerPrediction
{
	void Friction(Vector &velocity, bool isOnGround);
	bool CheckIsOnGround(CTFPlayer* player, Vector origin, Vector mins, Vector maxs);
	void AcceleratePlayer(Vector &velocity, Vector wishdir, float wishspeed, float accel);
	void AirAcceleratePlayer(Vector &velocity, Vector wishdir, float wishspeed, float accel, int surf);
	void StayOnGround(CTFPlayer* player, Vector &origin, Vector mins, Vector maxs, float stepsize);
	bool TryStepMove(CTFPlayer* player, Vector& origin, Vector& velocity, Vector mins, Vector maxs, float stepSize);
	void Predict(CTFPlayer* player, float time_seconds, std::vector<Vector> &path);
};