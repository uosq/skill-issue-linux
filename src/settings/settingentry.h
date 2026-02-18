#pragma once

#include "type.h"
#include <string>
#include <fstream>
#include <sstream>

class ISettingEntry
{
public:
	virtual ~ISettingEntry() = default;

	virtual SettingType GetType() const = 0;
	virtual const std::string& GetName() const = 0;

	virtual bool Save(const std::string& fullPath) = 0;
	virtual bool Load(const std::string& fullPath) = 0;
};

template<typename T>
class SettingEntry: public ISettingEntry
{
public:
	SettingEntry() = default;

	SettingEntry(T& ref, SettingType type, std::string name)
	{
		this->ptr = &ref;
		this->type = type;
		this->name = name;
	}

	SettingType GetType() const override { return type; }
	const std::string& GetName() const override { return name; }

	const T& GetValue() const { return *ptr; };
	void SetValue(const T& value) { *ptr = value; }

	bool Save(const std::string& fullPath) override;
	bool Load(const std::string& fullPath) override;

private:
	T* ptr;
	SettingType type;
	std::string name;
};

template<typename T>
bool SettingEntry<T>::Save(const std::string& fullPath)
{
	if (!ptr)
		return false;

	std::ofstream file(fullPath, std::ios::app);
	if (!file.is_open())
		return false;

	// if enum, cast to int
	// fuck C++, why do I need to do this shit?
	if constexpr (std::is_enum_v<T>)
	{
		file << name << "=" << static_cast<int>(*ptr) << "\n";
	}
	else
	{
		file << name << "=" << *ptr << "\n";
	}
	return true;
}

template<typename T>
bool SettingEntry<T>::Load(const std::string& fullPath)
{
	if (!ptr)
		return false;

	std::ifstream file(fullPath);
	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
	{
		auto pos = line.find('=');
		if (pos == std::string::npos)
			continue;

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		if (key == name)
		{
			std::stringstream ss(value);

			// same bullshit here
			if constexpr (std::is_enum_v<T>)
			{
				int temp;
				ss >> temp;
				*ptr = static_cast<T>(temp);
			}
			else
			{
				ss >> *ptr;
			}

			return true;
		}
	}

	return false;
}

// Had to make a separate one for std::string because
// >> stops at whitespace so we would be fucked
template<>
inline bool SettingEntry<std::string>::Load(const std::string& fullPath)
{
	if (!ptr)
		return false;

	std::ifstream file(fullPath);
	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
	{
		auto pos = line.find('=');
		if (pos == std::string::npos)
			continue;

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		if (key == name)
		{
			*ptr = value;
			return true;
		}
	}

	return false;
}