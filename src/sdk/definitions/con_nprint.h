//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Debug overlay / notfication printing 
//
//=============================================================================//

#ifndef CON_NPRINT_H
#define CON_NPRINT_H

#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Purpose:  Debug overlay / notfication printing 
// NOTE:  Structure cannot be changed by mods
//-----------------------------------------------------------------------------
typedef struct con_nprint_s
{
	int		index;			// Row #
	float	time_to_live;	// # of seconds before it disappears. -1 means to display for 1 frame then go away.
	float	color[ 3 ];		// RGB colors ( 0.0 -> 1.0 scale )
	bool	fixed_width_font;
} con_nprint_t;

#endif // CON_NPRINT_H