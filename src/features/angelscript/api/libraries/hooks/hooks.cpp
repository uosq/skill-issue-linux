#include "hooks.h"
#include "../../globals.h"

#include <unordered_map>

std::unordered_map<std::string, std::vector<ASHook>> &GetHookList()
{
	static std::unordered_map<std::string, std::vector<ASHook>> hooks;
	return hooks;
};

bool Hooks_HasHooks(const std::string &event)
{
	auto it = GetHookList().find(event);
	return it != GetHookList().end() && !it->second.empty();
}

bool Add(const std::string &event, const std::string &id, asIScriptFunction *func)
{
	if (!func)
		return false;
	auto &hooks = GetHookList()[event];

	for (const auto &entry : hooks)
	{
		if (entry.name == id)
			return false;
	}

	func->AddRef();
	hooks.push_back({id, func, func->GetModule()});
	return true;
}

bool Remove(const std::string &event, const std::string &id)
{
	auto &hooks = GetHookList();
	auto it	    = hooks.find(event);
	if (it == hooks.end())
		return false;

	auto &vec = it->second;
	for (auto vec_it = vec.begin(); vec_it != vec.end(); ++vec_it)
	{
		if (vec_it->name == id)
		{
			vec_it->func->Release();
			vec.erase(vec_it);
			return true;
		}
	}
	return false;
}

bool Hooks_GetHooks(const std::string &event, std::vector<ASHook> &out)
{
	auto &hooks = GetHookList();
	auto it	    = hooks.find(event);
	if (it == hooks.end())
		return false;

	out = it->second;

	return true;
}

void ExecuteHook(const ASHook &hook, const std::function<void(asIScriptContext *)> &argSetter)
{
	auto engine	      = GetScriptEngine();
	asIScriptContext *ctx = engine->RequestContext();

	ctx->Prepare(hook.func);

	// let caller set arguments however they want
	if (argSetter)
		argSetter(ctx);

	ctx->Execute();
	engine->ReturnContext(ctx);
}

void Hooks_CallHooks(const std::string &name, const std::function<void(asIScriptContext *)> &argSetter)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks(name, hooks))
		return;

	for (const auto &hook : hooks)
		ExecuteHook(hook, argSetter);
}

void Hooks_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->RegisterFuncdef("void CreateMoveHook(UserCmd@ cmd)");
	engine->RegisterFuncdef("void OverrideViewHook(ViewSetup@ view)");
	engine->RegisterFuncdef("void CalcViewModelHook(Vector3 position, Vector3 angle)");
	engine->RegisterFuncdef("void GenericHook()");
	engine->RegisterFuncdef("void LevelInitPreEntHook(const string &in mapName)");
	engine->RegisterFuncdef("void DrawModelHook(DrawModelContext@ ctx)");
	engine->RegisterFuncdef("void FireGameEventHook(GameEvent@ event)");

	engine->SetDefaultNamespace("Hooks");
	{
		// small amount of overloading
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_HOOKS);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "CreateMoveHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "GenericHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "CalcViewModelHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "OverrideViewHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "LevelInitPreEntHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "DrawModelHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Add(const string &in event, const string &in id, "
					       "FireGameEventHook @cb)",
					       asFUNCTION(Add), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Remove(const string &in event, const string &in id)",
					       asFUNCTION(Remove), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}

void Hooks_RemoveModule(asIScriptModule *module)
{
	for (auto &[event, vec] : GetHookList())
	{
		for (auto it = vec.begin(); it != vec.end();)
		{
			if (it->mod == module)
			{
				it->func->Release();
				it = vec.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}