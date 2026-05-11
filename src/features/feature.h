#pragma once

#define DECLARE_FEATURE(type, name) \
namespace features { \
	inline type name {}; \
}
