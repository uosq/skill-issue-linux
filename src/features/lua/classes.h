#pragma once

#include "../../sdk/definitions/types.h"
#include "../../sdk/classes/entity.h"
#include "../../sdk/definitions/inetmessage.h"
#include "../../sdk/definitions/inetchannel.h"
#include "../../sdk/definitions/igameevents.h"
#include "../../imgui/imgui.h"

#include "pluto/lua.h"
#include "pluto/lua.hpp"
#include "pluto/lualib.h"

#include "classes/usercmdlua.h"
#include "classes/vectorlua.h"
#include "classes/entitylua.h"
#include "classes/stringcmdlua.h"

// Reference instead of the real ones
// Or else lua might cook us

struct LuaMaterial
{
	IMaterial* mat;
	std::string name;
};

struct LuaBuffer
{
	bf_write* writer;
	bf_read* reader;

	unsigned char* data;
	int curbitpos = 0;
};

struct LuaNetMessage
{
	INetMessage* msg;
};

struct LuaTexture
{
	ITexture* tex;
	std::string name;
};

struct LuaNetChannel
{
	CNetChannel* netchan;
};

struct LuaGameEvent
{
	IGameEvent* event;
};

struct LuaViewSetup
{
	CViewSetup* view;
};

namespace LuaClasses
{
	namespace MaterialLua
	{
		extern const luaL_Reg methods[];
		void luaopen_material(lua_State* L);
		LuaMaterial* push_material(lua_State* L, IMaterial* mat, const std::string& name);

		int Index(lua_State* L);
		int GC(lua_State* L);
		int ToString(lua_State* L);

		int SetColorModulation(lua_State* L);
		int GetColorModulation(lua_State* L);

		int SetMaterialVarFlag(lua_State* L);
		int GetMaterialVarFlag(lua_State* L);

		int GetTextureGroupName(lua_State* L);
		int GetName(lua_State* L);

		int Delete(lua_State* L);
	}

	namespace BitBufferLua
	{
		extern const luaL_Reg methods[];
		void luaopen_buffer(lua_State* L);
		LuaBuffer* push_buffer(lua_State* L, bf_write* writer, bf_read* reader);
		int BitBuffer(lua_State* L);

		int Index(lua_State* L);
		int GC(lua_State* L);

		int SetCurBitPos(lua_State* L);
		int GetCurBitPos(lua_State* L);

		int Reset(lua_State* L);
		
		int WriteBit(lua_State* L);
		int ReadBit(lua_State* L);

		int WriteByte(lua_State* L);
		int ReadByte(lua_State* L);

		int WriteString(lua_State* L);
		int ReadString(lua_State* L);

		int WriteFloat(lua_State* L);
		int ReadFloat(lua_State* L);

		int WriteInt(lua_State* L);
		int ReadInt(lua_State* L);
		
		int Delete(lua_State* L);
	}

	namespace NetMessageLua
	{
		extern const luaL_Reg methods[];
		void luaopen_netmessage(lua_State* L);
		LuaNetMessage* push_netmessage(lua_State* L, INetMessage* msg);

		int Index(lua_State* L);
		int GC(lua_State* L);

		int GetType(lua_State* L);
		int GetName(lua_State* L);
		int ToString(lua_State* L);
		int WriteToBitBuffer(lua_State* L);
		int ReadFromBitBuffer(lua_State* L);
		int IsReliable(lua_State* L);
		int SetReliable(lua_State* L);
		int GetGroup(lua_State* L);
	}

	namespace TextureLua
	{
		extern const luaL_Reg methods[];
		void luaopen_texture(lua_State* L);
		LuaTexture* push_texture(lua_State* L, ITexture* tex, const std::string& name);

		int Index(lua_State* L);
		int GC(lua_State* L);
		int ToString(lua_State* L);

		int Delete(lua_State* L);
		int GetName(lua_State* L);
		int GetFlags(lua_State* L);
		int GetActualWidth(lua_State* L);
		int GetActualHeight(lua_State* L);
		int IsTranslucent(lua_State* L);
		int IsRenderTarget(lua_State* L);
	}

	namespace NetChannelLua
	{
		extern const luaL_Reg methods[];
		void luaopen_netchannel(lua_State* L);
		LuaNetChannel* push_netchannel(lua_State* L, CNetChannel* netchan);

		int Index(lua_State* L);
		int GC(lua_State* L);

		int SetDataRate(lua_State* L);
		int RegisterMessage(lua_State* L);
		int SetTimeout(lua_State* L);
		int SetChallengeNr(lua_State* L);
		int ProcessPlayback(lua_State* L);
		int SendNetMsg(lua_State* L);
		int SendData(lua_State* L);
		int SendFile(lua_State* L);
		int DenyFile(lua_State* L);
		int SetChoked(lua_State* L);
		int SendDatagram(lua_State* L);
		int Transmit(lua_State* L);
		int GetDropNumber(lua_State* L);
		int GetSocket(lua_State* L);
		int GetChallengeNr(lua_State* L);
		int GetSequenceData(lua_State* L);
		int SetSequenceData(lua_State* L);
		int UpdateMessageStats(lua_State* L);
		int CanPacket(lua_State* L);
		int IsOverflowed(lua_State* L);
		int IsTimedOut(lua_State* L);
		int HasPendingReliableData(lua_State* L);
		int SetFileTransmissionMode(lua_State* L);
		int SetCompressionMode(lua_State* L);
		int RequestFile(lua_State* L);
		int GetTimeSinceLastReceived(lua_State* L);
		int SetMaxBufferSize(lua_State* L);
		int IsNull(lua_State* L);
		int GetNumBitsWritten(lua_State* L);
		int SetInterpolationAmount(lua_State* L);
		int SetRemoteFramerate(lua_State* L);
		int SetMaxRoutablePayloadSize(lua_State* L);
		int GetMaxRoutablePayloadSize(lua_State* L);
		int GetProtocolVersion(lua_State* L);
	}

	namespace GameEventLua
	{
		extern const luaL_Reg methods[];
		void luaopen_gameevent(lua_State* L);
		LuaGameEvent* push_gameevent(lua_State* L, IGameEvent* event);

		int Index(lua_State* L);
		int GC(lua_State* L);

		int GetName(lua_State* L);

		int IsReliable(lua_State* L);
		int IsLocal(lua_State* L);
		int IsEmpty(lua_State* L);
		
		int GetBool(lua_State* L);
		int GetInt(lua_State* L);
		int GetFloat(lua_State* L);
		int GetString(lua_State* L);

		int SetBool(lua_State* L);
		int SetInt(lua_State* L);
		int SetFloat(lua_State* L);
		int SetString(lua_State* L);
	}

	namespace ViewSetupLua
	{
		void luaopen_viewsetup(lua_State* L);
		LuaViewSetup* push_viewsetup(lua_State* L, CViewSetup* view);

		int Index(lua_State* L);
		int NewIndex(lua_State* L);
	}
}