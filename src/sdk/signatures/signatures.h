#pragma once

#include "../../libsigscan.h"
#include <string>
#include <vector>

class Sig
{
      public:
	Sig(const std::string &name, const std::string &dll, const std::string &pattern)
	{
		this->name    = name;
		this->dll     = dll;
		this->pattern = pattern;
		this->ptr     = nullptr;

		GetRegistry().emplace_back(this);
	}

	bool Init()
	{
		ptr = sigscan_module(dll.c_str(), pattern.c_str());
		return ptr != nullptr;
	}

	static std::vector<Sig *> &GetRegistry()
	{
		static std::vector<Sig *> registry;
		return registry;
	}

	void *GetPointer()
	{
		return ptr;
	}

	const std::string &GetDLL()
	{
		return dll;
	}

	const std::string &GetPattern()
	{
		return pattern;
	}

	const std::string &GetName()
	{
		return name;
	}

      private:
	std::string dll	    = "";
	std::string pattern = "";
	std::string name    = "";
	void *ptr	    = nullptr;
};

#define ADD_SIG(name, dll, pattern)                                                                                    \
	namespace Sigs                                                                                                 \
	{                                                                                                              \
		inline Sig name{#name, dll, pattern};                                                                  \
	}

bool Sigs_InitAll();