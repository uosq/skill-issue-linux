#pragma once

#include <array>

#include "../../thirdparty/funchook/funchook.h"
#include "../../thirdparty/libsigscan/libsigscan.h"
#include "../../sdk/interfaces/interfaces.h"

class CHookManager
{
public:
	using HookFn = void(*)();

	static void AddHook(HookFn hook);
	static void RemoveHook(HookFn hook);

	static void LoadAllHooks();
	static void UnloadHooks();

	static constexpr int s_nMaxHooks = 64;

	static int s_nHookCount;
	static funchook_t* s_pFuncHook;
	static std::array<HookFn, s_nMaxHooks> s_vecHooks;
};

struct CHookRegister
{
	explicit CHookRegister(const CHookManager::HookFn fn)
	{
		CHookManager::AddHook(fn);
	}
};

#define REGISTER_HOOK(fn) CHookRegister Hook_## fn {fn};
#define PREPARE_HOOK(original_fn, hook_fn) funchook_prepare(CHookManager::s_pFuncHook, reinterpret_cast<void**>(&(original_fn)), reinterpret_cast<void*>(hook_fn))

#define INIT_HOOK(name, ret, args, module, signature) \
	using name## Fn = ret(*) args; \
	static name## Fn original_## name = nullptr; \
	\
	ret name args; \
	\
	static void Init_## name()                                         \
	{                                                                 \
		original_## name = reinterpret_cast<name## Fn>(sigscan_module(module, signature)); \
		if (!original_## name) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		if (PREPARE_HOOK(original_## name, name) != FUNCHOOK_ERROR_SUCCESS) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
	} \
\
REGISTER_HOOK(Init_## name) \
ret name args

#define INIT_DLSYM_HOOK(name, ret, args, handle, symbol) \
using name## Fn = ret(*) args; \
static name## Fn original_## name = nullptr; \
\
ret name args; \
\
static void Init_## name() \
{ \
	void* lib = dlopen(handle, RTLD_NOLOAD | RTLD_LAZY); \
	\
	original_## name = reinterpret_cast<name## Fn>(dlsym(lib, symbol)); \
	\
	if (!original_## name) \
	{ \
		interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
		return; \
	} \
	\
	if (PREPARE_HOOK(original_## name, name) != FUNCHOOK_ERROR_SUCCESS) \
	{ \
		interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
		return; \
	} \
} \
\
REGISTER_HOOK(Init_## name) \
\
ret name args

#define INIT_HOOK_OFFSET(name, ret, args, module, signature, offset) \
	using name## Fn = ret(*) args; \
	static name## Fn original_## name = nullptr; \
	\
	ret name args; \
	\
	static void Init_## name() \
	{ \
		void* match = sigscan_module(module, signature); \
		if (!match) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		original_## name = reinterpret_cast<name## Fn>(reinterpret_cast<uintptr_t>(match) - (offset)); \
		\
		if (PREPARE_HOOK(original_## name, name) != FUNCHOOK_ERROR_SUCCESS) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
	} \
\
REGISTER_HOOK(Init_## name) \
ret name args

#define INIT_VTABLE_HOOK(name, ret, args, instance, index) \
	using name## Fn = ret(*) args; \
	static name## Fn original_## name = nullptr; \
	\
	ret name args; \
	\
	static void Init_## name() \
	{ \
		void* pInst = reinterpret_cast<void*>(instance); \
		if (!pInst) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		void** vtable = *reinterpret_cast<void***>(pInst); \
		if (!vtable) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		original_## name = reinterpret_cast<name## Fn>(vtable[index]); \
		if (!original_## name) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		if (PREPARE_HOOK(original_## name, name) != FUNCHOOK_ERROR_SUCCESS) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
	} \
\
REGISTER_HOOK(Init_## name) \
ret name args

#define INIT_VTABLE_OFFSET_HOOK(name, ret, args, instance, offset) \
	using name## Fn = ret(*) args; \
	static name## Fn original_## name = nullptr; \
	\
	ret name args; \
	\
	static void Init_## name() \
	{ \
		void* pInst = reinterpret_cast<void*>(instance); \
		if (!pInst) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		void** vtable = *reinterpret_cast<void***>(pInst); \
		if (!vtable) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		original_## name = *reinterpret_cast<name## Fn*>(reinterpret_cast<uintptr_t>(vtable) + (offset)); \
		if (!original_## name) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
		\
		if (PREPARE_HOOK(original_## name, name) != FUNCHOOK_ERROR_SUCCESS) \
		{ \
			interfaces::Cvar->ConsolePrintf("%s\n", "Couldn't hook " #name); \
			return; \
		} \
	} \
\
REGISTER_HOOK(Init_## name) \
ret name args
