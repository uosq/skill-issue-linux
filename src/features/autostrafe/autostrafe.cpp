#include "autostrafe.h"


namespace Autostrafe
{

        float get_sv_airaccelerate()
        {
            static ConVar * sv_airaccelerate = interfaces::Cvar->FindVar("sv_airaccelerate");
            return sv_airaccelerate->GetFloat();
        }

        float get_sv_maxspeed()
        {
            static ConVar * sv_maxspeed = interfaces::Cvar->FindVar("sv_maxspeed");
            return sv_maxspeed->GetFloat();
        }

        float DeltaRadAngle(float a, float b)
        {
            float delta = isfinite(a - b) ? remainder(a - b, 360) : 0;

            if (a > b && delta >= M_PI)
            {
                delta -= M_PI * 2;
            }
            else if (delta <= -M_PI)
            {
                delta += M_PI * 2;
            }

            return delta;
        }

	void Run(CTFPlayer* pLocal, CUserCmd* pCmd)
	{
		if (!Settings::Misc.autostrafe || pLocal->GetWaterLevel() > WL_Feet)
			return;
            
                bool isOnGround = pLocal->GetFlags() & FL_ONGROUND;
                bool isOnWater = pLocal->GetFlags() & FL_INWATER;
		bool isScout = pLocal->m_iClass() == TF_CLASS_SCOUT;

                /* could not manage to make it work nicely on scout */
                if (isOnGround || isOnWater || isScout) return;


                Vec3 velocity = pLocal->GetVelocity();
                const float speed = velocity.Length();

                if (speed < 30) return;

                float sv_airaccelerate = get_sv_airaccelerate();
                float sv_maxspeed = get_sv_maxspeed();

                /* assume some default values */
                const float cl_forwardsspeed = 450.0f;
                const float cl_sidespeed = 450.0f;

                float terminal = sv_airaccelerate / sv_maxspeed * 100.0f / speed;

                if (terminal < -1 || terminal > 1) return;

                float good_delta = acosf(terminal);

                Vec3 viewAngles;
		interfaces::Engine->GetViewAngles(viewAngles);

                float yaw = DEG2RAD(viewAngles.y);
                float vel_direction = atan2f(velocity.y, velocity.x) - yaw;
                float target_angle = atan2f(-pCmd->sidemove, pCmd->forwardmove);
                float delta = DeltaRadAngle(vel_direction, target_angle);

                float move_direction = delta < 0 ? vel_direction + good_delta : vel_direction - good_delta;

                pCmd->forwardmove = cosf(move_direction) * cl_forwardsspeed;
                pCmd->sidemove = -sinf(move_direction) * cl_sidespeed;

	}
}
