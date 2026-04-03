#include "globals.h"

// I don't like singletons, but I couldn't
// think of a better way of doing it
asIScriptContext *GetScriptContext()
{
	static asIScriptContext *ctx = GetScriptEngine()->CreateContext();
	return ctx;
}

asIScriptEngine *GetScriptEngine()
{
	static asIScriptEngine *engine = asCreateScriptEngine();
	return engine;
}

int &GetScriptAccessMask()
{
	static int mask = -1;
	return mask;
}