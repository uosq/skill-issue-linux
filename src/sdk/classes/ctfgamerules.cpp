#include "ctfgamerules.h"

CTFGameRules* TFGameRules()
{
	static uintptr_t leaInstr = reinterpret_cast<uintptr_t>(sigscan_module("client.so", "48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 ? 31 C0"));
	static uintptr_t g_pGameRules_addr = RelToAbs(leaInstr, 3, 7);
	static CTFGameRules *g_pGameRules  = *reinterpret_cast<CTFGameRules **>(g_pGameRules_addr);
	return g_pGameRules;
}