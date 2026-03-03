#pragma once

#include <cstdint>
#include <string_view>
#include <variant>
#include "type.h"

struct SettingEntry
{
	std::string_view name;
	SettingType type;
	void* ptr;
};