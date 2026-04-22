#include "itemschema.h"

#include <cstdint>

void* GetItemSchema()
{
	/*
	xref: featured_item_index
	bool CEconStorePriceSheet::InitFromKV( KeyValues *pKVRoot )
	
	    if (cVar2 != '\0') {
	      uVar4 = FUN_01fddec0(param_2,"featured_item_index",0);
	      *(undefined4 *)(param_1 + 0x3a) = uVar4;
======>	      uVar7 = GetItemSchema();
	      uVar8 = FUN_01fde460(param_2,"promotion_first_time_purchase_gift",&DAT_02430203);
	      uVar7 = FUN_01ab8b00(uVar7,uVar8);
	      param_1[0x34] = uVar7;
======>	      uVar7 = GetItemSchema();
	      uVar8 = FUN_01fde460(param_2,"promotion_first_time_web_purchase_gift",&DAT_02430203);
	      uVar7 = FUN_01ab8b00(uVar7,uVar8);
	*/

	using GetItemSchemaFn = void*(*)();
	static GetItemSchemaFn original = (GetItemSchemaFn)sigscan_module("client.so", "48 8B 05 ? ? ? ? 55 48 89 E5 41 55 41 54 48 85 C0");

	if (!original)
		return nullptr;

	return original();
}

void* ItemSchema_GetItemDefinition(int iItemDefinitionIndex)
{
	/*
	xref: TFUI_InvTooltip_ItemFound_Itemname
	ClientModeShared::FireGameEvent
	
	if ((!bVar29 && !bVar26) == bVar29) {
		uVar4 = (**(code **)(*gameEvent + 0x38))(gameEvent,"player",0);
		iVar5 = (**(code **)(*gameEvent + 0x38))(gameEvent,"quality",0);
		uVar8 = (**(code **)(*gameEvent + 0x38))(gameEvent,"method",0);
		lVar20 = (long)(int)uVar8;
		uVar7 = (**(code **)(*gameEvent + 0x38))(gameEvent,"itemdef",0);
		iVar6 = (**(code **)(*gameEvent + 0x38))(gameEvent,"isstrange",0);
		iVar9 = (**(code **)(*gameEvent + 0x38))(gameEvent,"isunusual",0);
		(**(code **)(*gameEvent + 0x40))(0,gameEvent,"wear");
		lVar19 = FUN_017ba0d0(uVar4);
		uVar12 = GetItemSchema();
=====>		lVar16 = ItemSchema_GetItemDefinition(uVar12,uVar7);
		if (lVar16 == 0) {
		return;
		}
		lVar16 = __dynamic_cast(lVar16,&CEconItemDefinition::typeinfo,&CTFItemDefinition::typeinfo,0);
		if (lVar19 == 0) {
		return;
	}
	*/

	using GetItemDefinitionFn = void*(*)(void* pSchema, int iItemDefinitionIndex);
	static GetItemDefinitionFn original = (GetItemDefinitionFn)sigscan_module("client.so", "55 48 89 E5 41 55 41 54 49 89 FC 53 48 83 EC 08 8B 87 4C 01 00 00");

	void* pSchema = GetItemSchema();

	if (pSchema == nullptr)
		return nullptr;

	return original(pSchema, iItemDefinitionIndex);
}

const char* ItemDefinition_GetBaseName(int iItemDefinitionIndex)
{
	/*
	void __thiscall CEconPreviewExpiredNotification::Trigger(CEconPreviewExpiredNotification *this)
	{
		undefined8 uVar1;
		undefined8 uVar2;
		
		uVar1 = FUN_01b33210("#TF_PreviewItem_Expired_Title","#TF_PreviewItem_Expired_Text",
				"#TF_PreviewItem_BuyIt","#TF_PreviewItem_NotNow",g_bOpenStoreToItem,0,0);
		FUN_01b33200(uVar1,this,0);
		uVar2 = (**(code **)(*g_pVGuiLocalize + 0x10))
0x58 =>				(g_pVGuiLocalize,*(undefined8 *)(*(long *)(this + 0x1040) + 0x58));
		FUN_01b33180(uVar1,"item_name",uVar2);
		FUN_01b44d20(this,1);
		return;
	}
	*/

	void* pItemDef = ItemSchema_GetItemDefinition(iItemDefinitionIndex);

	if (pItemDef == nullptr)
		return nullptr;

	return *reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(pItemDef) + 0x58);
}