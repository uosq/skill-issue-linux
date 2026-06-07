#pragma once

#include <stdint.h>

#include "../../thirdparty/libsigscan/libsigscan.h"
#include "../interfaces/interfaces.h"
#include "types.h"

/*
sigs
KeyValues::LoadFromBuffer
55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 85 D2

KeyValues::KeyValues
55 31 C0 66 0F EF C0 48 89 E5 53
*/

/*

The function FX_MuzzleEffectAttached has a lot of KeyValues functions (SetInt, SetFloat, SetString, SetPtr)

*/

enum types_t
{
	TYPE_NONE = 0,
	TYPE_STRING,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_PTR,
	TYPE_WSTRING,
	TYPE_COLOR,
	TYPE_UINT64,
	TYPE_NUMTYPES,
};

class KeyValues
{
      private:
	int m_iKeyName;
	char *m_sValue;
	wchar_t *m_wsValue;

	union {
		int m_iValue;
		float m_flValue;
		void *m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences;
	char m_bEvaluateConditionals;
	char unused[1];

	KeyValues *m_pPeer;
	KeyValues *m_pSub;
	KeyValues *m_pChain;

      public:
	class AutoDelete
	{
	      public:
		explicit inline AutoDelete(KeyValues *pKeyValues) : m_pKeyValues(pKeyValues)
		{
		}
		explicit inline AutoDelete(const char *pchKVName) : m_pKeyValues(new KeyValues(pchKVName))
		{
		}
		inline ~AutoDelete(void)
		{
			if (m_pKeyValues)
				m_pKeyValues->DeleteThis();
		}
		inline void Assign(KeyValues *pKeyValues)
		{
			m_pKeyValues = pKeyValues;
		}
		KeyValues *operator->()
		{
			return m_pKeyValues;
		}
		operator KeyValues *()
		{
			return m_pKeyValues;
		}

	      private:
		AutoDelete(AutoDelete const &x);	    // forbid
		AutoDelete &operator=(AutoDelete const &x); // forbid
		KeyValues *m_pKeyValues;
	};

	bool LoadFromBuffer(char const *resource_name, const char *buffer, void *file_system = 0,
			    const char *path_id = 0)
	{
		using LoadFromBufferFn = bool(void *self, char const *, const char *, void *, const char *);
		static auto orig       = (LoadFromBufferFn *)sigscan_module(
			  "engine.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 85 D2");
		return orig(this, resource_name, buffer, file_system, path_id);
	}

	void Initialize(const char *name)
	{
		m_iKeyName = INVALID_KEY_SYMBOL;
		m_iDataType = TYPE_NONE;

		m_pSub = NULL;
		m_pPeer = NULL;
		m_pChain = NULL;

		m_sValue = NULL;
		m_wsValue = NULL;
		m_pValue = NULL;
		
		m_bHasEscapeSequences = false;
		m_bEvaluateConditionals = true;

		// for future proof
		memset( unused, 0, sizeof(unused) );
	}

	KeyValues(const char *name)
	{
		Initialize(name);
	}

	void *operator new(size_t iAllocSize)
	{
		return interfaces::KeyValuesSystem->AllocKeyValuesMemory((int)iAllocSize);
	}

	void *operator new(size_t iAllocSize, int nBlockUse, const char *pFileName, int nLine)
	{
		void *p = interfaces::KeyValuesSystem->AllocKeyValuesMemory((int)iAllocSize);
		return p;
	}

	void operator delete(void *pMem)
	{
		interfaces::KeyValuesSystem->FreeKeyValuesMemory(pMem);
	}

	void operator delete(void *pMem, int nBlockUse, const char *pFileName, int nLine)
	{
		interfaces::KeyValuesSystem->FreeKeyValuesMemory(pMem);
	}

	void DeleteThis()
	{
		delete this;
	}

	void SetString(const char* keyName, const char* value)
	{
		using orig_KeyValues_SetString = void(*)(KeyValues*, const char*, const char*);
		static orig_KeyValues_SetString original = reinterpret_cast<orig_KeyValues_SetString>(sigscan_module("client.so", "55 48 89 E5 41 55 41 54 49 89 D4 BA 01 00 00 00 53 48 83 EC 08 E8 ? ? ? ? 48 85 C0 74 ? 80 78 20 01"));

		original(this, keyName, value);
	}

	KeyValues* FindKey(const char* keyName, bool bCreate = false)
	{
		using orig_KeyValues_FindKey = KeyValues*(*)(KeyValues*, const char*, bool);
		static orig_KeyValues_FindKey original = reinterpret_cast<orig_KeyValues_FindKey>(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 89 FB 48 81 EC 18 01 00 00 48 85 F6"));

		return original(this, keyName, bCreate);
	}

	KeyValues* FindKey(int keySymbol) const
	{
		for (KeyValues *dat = m_pSub; dat != nullptr; dat = dat->m_pPeer)
		{
			if (dat->m_iKeyName == keySymbol)
				return dat;
		}

		return nullptr;
	}
};

typedef KeyValues::AutoDelete KeyValuesAD;