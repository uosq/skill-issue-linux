#pragma once

#include <array>
#include <string_view>
#include <variant>
#include <vector>

#include "../../sdk/definitions/color.h"
#include "../../sdk/definitions/types.h"

#include "../binds/binds.h"
#include "../feature.h"

class IConfigValue
{
public:
	virtual ~IConfigValue() = default;
	virtual std::string_view GetName() const = 0;
	using ValuePtr = std::variant<bool*, int*, float*, Color*, Hotkey*, std::array<float, 3>*, std::array<float, 4>*>;
	virtual ValuePtr GetRef() = 0;

	inline static IConfigValue* g_pHead = nullptr;
	IConfigValue* next = nullptr;

protected:
	void RegisterSelf() {
		this->next = g_pHead;
		g_pHead = this;
	}
};

template <typename T>
class ConfigValue : public IConfigValue
{
public:
	std::string_view name;
	T value;

	ConfigValue(std::string_view display_name, T default_value)
	    : name(display_name), value(default_value)
	{
		RegisterSelf();

		if constexpr (std::is_same_v<T, Hotkey>)
		{
			value.m_strName = display_name;
			features::binds.RegisterHotkey(&value);
		}
	}

	T& Get() { return value; }
	const T& Get() const { return value; }
	void Set(const T& val) { value = val; }

	std::string_view GetName() const override { return name; }

	ValuePtr GetRef() override { return &value; }
};

template <typename T>
constexpr auto CreateConfigValue(std::string_view display, T default_value)
{
	return ConfigValue<T>{display, default_value};
}

#define BEGIN_CONFIG(name) namespace config { namespace name {
#define END_CONFIG() }}
#define ADD_CONFIG(name, display, value) inline auto name = CreateConfigValue(#display, value);

ADD_CONFIG(menu_open, "menu open", false)