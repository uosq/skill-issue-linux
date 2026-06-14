#pragma once

#include "../config/config.h"

BEGIN_CONFIG(aspect_ratio)
	ADD_CONFIG(value, "aspect ratio", 0.0f)
END_CONFIG()

BEGIN_CONFIG(viewmodel_bob)
	ADD_CONFIG(enabled, "viewmodel bob enabled", false)
END_CONFIG()

BEGIN_CONFIG(sv_pure_bypass)
	ADD_CONFIG(enabled, "sv_pure bypass", false)
END_CONFIG()

BEGIN_CONFIG(backpack_expander)
	ADD_CONFIG(enabled, "backpack expander", false)
END_CONFIG()

BEGIN_CONFIG(no_scope_overlay)
	ADD_CONFIG(enabled, "no scope overlay", false)
END_CONFIG()

BEGIN_CONFIG(no_zoom)
	ADD_CONFIG(enabled, "no zoom", false)
END_CONFIG()

BEGIN_CONFIG(no_engine_sleep)
	ADD_CONFIG(enabled, "no engine sleep", false)
END_CONFIG()

BEGIN_CONFIG(streamer_mode)
	ADD_CONFIG(enabled, "streamer mode", false)
END_CONFIG()

BEGIN_CONFIG(accept_item_drop)
	ADD_CONFIG(enabled, "accept item drop", false)
END_CONFIG()