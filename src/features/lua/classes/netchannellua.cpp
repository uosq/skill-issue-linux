#include "../classes.h"
#include "../pluto/lauxlib.h"
#include "../pluto/lua.h"

namespace LuaClasses
{
	namespace NetChannelLua
	{
		const luaL_Reg methods[]
		{
			{"SetDataRate", SetDataRate},
			{"RegisterMessage", RegisterMessage},
			{"SetTimeout", SetTimeout},
			{"SetChallengeNr", SetChallengeNr},
			{"ProcessPlayback", ProcessPlayback},
			{"SendNetMsg", SendNetMsg},
			{"SendData", SendData},
			{"SendFile", SendFile},
			{"DenyFile", DenyFile},
			{"SetChoked", SetChoked},
			{"SendDatagram", SendDatagram},
			{"Transmit", Transmit},
			{"GetDropNumber", GetDropNumber},
			{"GetSocket", GetSocket},
			{"GetChallengeNr", GetChallengeNr},
			{"GetSequenceData", GetSequenceData},
			{"SetSequenceData", SetSequenceData},
			{"UpdateMessageStats", UpdateMessageStats},
			{"CanPacket", CanPacket},
			{"IsOverflowed", IsOverflowed},
			{"IsTimedOut", IsTimedOut},
			{"HasPendingReliableData", HasPendingReliableData},
			{"SetFileTransmissionMode", SetFileTransmissionMode},
			{"SetCompressionMode", SetCompressionMode},
			{"RequestFile", RequestFile},
			{"GetTimeSinceLastReceived", GetTimeSinceLastReceived},
			{"SetMaxBufferSize", SetMaxBufferSize},
			{"IsNull", IsNull},
			{"GetNumBitsWritten", GetNumBitsWritten},
			{"SetInterpolationAmount", SetInterpolationAmount},
			{"SetRemoteFramerate", SetRemoteFramerate},
			{"SetMaxRoutablePayloadSize", SetMaxRoutablePayloadSize},
			{"GetMaxRoutablePayloadSize", GetMaxRoutablePayloadSize},
			{"GetProtocolVersion", GetProtocolVersion},
			{nullptr, nullptr}
		};

		void luaopen_netchannel(lua_State* L)
		{
			luaL_newmetatable(L, "NetChannel");

			luaL_setfuncs(L, methods, 0);

			lua_pushcfunction(L, Index);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, GC);
			lua_setfield(L, -2, "__gc");

			lua_pop(L, 1);
		}

		LuaNetChannel* push_netchannel(lua_State* L, CNetChannel* netchan)
		{
			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(lua_newuserdata(L, sizeof(LuaNetChannel)));
			lchan->netchan = netchan;

			luaL_getmetatable(L, "NetChannel");
			lua_setmetatable(L, -2);

			return lchan;
		}

		int Index(lua_State* L)
		{
			luaL_getmetatable(L, "NetChannel");
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			return 1;
		}

		int GC(lua_State* L)
		{
			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			lchan->netchan = nullptr;
			return 0;
		}

		int SetDataRate(lua_State* L)
		{
			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "Expected NetChannel, received nil");
				return 0;
			}

			float rate = luaL_checknumber(L, 1);
			lchan->netchan->SetDataRate(rate);

			return 0;
		}

		int RegisterMessage(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			if (lua_isnoneornil(L, 2))
			{
				luaL_error(L, "Argument #2: Expected NetMessage, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			LuaNetMessage* lmsg = static_cast<LuaNetMessage*>(luaL_checkudata(L, 2, "NetMessage"));
			if (lmsg->msg == nullptr)
			{
				luaL_error(L, "NetMessage is nullptr");;
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->RegisterMessage(lmsg->msg));
			return 1;
		}

		int SetTimeout(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			float seconds = luaL_checknumber(L, 2);
			lchan->netchan->SetTimeout(seconds);
			return 0;
		}

		int SetChallengeNr(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			unsigned int chnr = luaL_checkinteger(L, 1);
			lchan->netchan->SetChallengeNr(chnr);
			return 0;
		}

		int ProcessPlayback(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lchan->netchan->ProcessPlayback();
			return 0;
		}

		int SendNetMsg(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			if (lua_isnoneornil(L, 2))
			{
				luaL_error(L, "Argument #2: Expected NetMessage, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			LuaNetMessage* lmsg = static_cast<LuaNetMessage*>(luaL_checkudata(L, 2, "NetMessage"));
			if (lmsg->msg == nullptr)
			{
				luaL_error(L, "NetMessage is nullptr");
				return 0;
			}

			bool bForceReliable = luaL_optinteger(L, 3, 0);
			bool bVoice = luaL_optinteger(L, 4, 0);

			lua_pushboolean(L, lchan->netchan->SendNetMsg(*lmsg->msg, bForceReliable, bVoice));
			return 1;
		}

		int SendData(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			if (lua_isnoneornil(L, 2))
			{
				luaL_error(L, "Argument #2: Expected BitBuffer, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			LuaBuffer* lbf = static_cast<LuaBuffer*>(luaL_checkudata(L, 2, "BitBuffer"));
			if (lbf->reader == nullptr || lbf->writer == nullptr)
			{
				luaL_error(L, "LuaBuffer is nullptr");
				return 0;
			}

			bool bReliable = luaL_optinteger(L, 3, 1);
			lchan->netchan->SendData(*lbf->writer, bReliable);
			return 0;
		}

		int SendFile(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			const char* filename = luaL_checkstring(L, 2);
			unsigned int transferID = luaL_checkinteger(L, 3);
			lua_pushboolean(L, lchan->netchan->SendFile(filename, transferID));
			return 1;
		}

		int DenyFile(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			const char* filename = luaL_checkstring(L, 2);
			unsigned int transferID = luaL_checkinteger(L, 3);

			lchan->netchan->DenyFile(filename, transferID);
			return 0;
		}

		int SetChoked(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lchan->netchan->SetChoked();
			return 0;
		}

		int SendDatagram(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			if (lua_isnoneornil(L, 2))
			{
				luaL_error(L, "Argument #2: Expected BitBuffr, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			LuaBuffer* lb = static_cast<LuaBuffer*>(luaL_checkudata(L, 2, "BitBuffer"));
			if (lb->writer == nullptr)
			{
				luaL_error(L, "BitBuffer writer is nullptr");
				return 0;
			}

			lchan->netchan->SendDatagram(lb->writer);
			return 0;
		}

		int Transmit(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			bool bOnlyReliable = luaL_optinteger(L, 2, 0);
			lchan->netchan->Transmit(bOnlyReliable);
			return 0;
		}

		int GetDropNumber(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			int drop = lchan->netchan->GetDropNumber();
			lua_pushinteger(L, drop);
			return 1;
		}

		int GetSocket(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushinteger(L, lchan->netchan->GetSocket());
			return 1;
		}

		int GetChallengeNr(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushinteger(L, lchan->netchan->GetChallengeNr());
			return 1;
		}

		int GetSequenceData(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			int nOutSequenceNr = 0;
			int nInSequenceNr = 0;
			int nOutSequenceNrAck = 0;
			lchan->netchan->GetSequenceData(nOutSequenceNr, nInSequenceNr, nOutSequenceNrAck);

			lua_pushinteger(L, nOutSequenceNr);
			lua_pushinteger(L, nInSequenceNr);
			lua_pushinteger(L, nOutSequenceNrAck);
			return 3;
		}

		int SetSequenceData(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			int nOutSequenceNr = luaL_checkinteger(L, 2);
			int nInSequenceNr = luaL_checkinteger(L, 3);
			int nOutSequenceNrAck = luaL_checkinteger(L, 4);
			lchan->netchan->SetSequenceData(nOutSequenceNr, nInSequenceNr, nOutSequenceNrAck);
			return 0;
		}

		int UpdateMessageStats(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			int msggroup = luaL_checkinteger(L, 2);
			int bits = luaL_checkinteger(L, 3);
			lchan->netchan->UpdateMessageStats(msggroup, bits);
			return 0;
		}

		int CanPacket(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->CanPacket());
			return 1;
		}

		int IsOverflowed(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->IsOverflowed());
			return 1;
		}

		int IsTimedOut(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->IsTimedOut());
			return 1;
		}

		int HasPendingReliableData(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->HasPendingReliableData());
			return 1;
		}

		int SetFileTransmissionMode(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			bool bBackgroundMode = static_cast<bool>(luaL_checkinteger(L, 2));
			lchan->netchan->SetFileTransmissionMode(bBackgroundMode);
			return 0;
		}

		int SetCompressionMode(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			bool bUseCompression = static_cast<bool>(luaL_checkinteger(L, 2));
			lchan->netchan->SetCompressionMode(bUseCompression);
			return 0;
		}

		int RequestFile(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			const char* filename = luaL_checkstring(L, 2);
			lua_pushinteger(L, lchan->netchan->RequestFile(filename));
			return 1;
		}

		int GetTimeSinceLastReceived(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushnumber(L, lchan->netchan->GetTimeSinceLastReceived());
			return 1;
		}

		int SetMaxBufferSize(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			bool bReliable = static_cast<bool>(luaL_checkinteger(L, 2));
			int nBytes = luaL_checkinteger(L, 3);
			bool bVoice = static_cast<bool>(luaL_checkinteger(L, 4));
			lchan->netchan->SetMaxBufferSize(bReliable, nBytes, bVoice);
			return 0;
		}

		int IsNull(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushboolean(L, lchan->netchan->IsNull());
			return 1;
		}

		int GetNumBitsWritten(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			bool bReliable = static_cast<bool>(luaL_checkinteger(L, 2));
			lua_pushinteger(L, lchan->netchan->GetNumBitsWritten(bReliable));
			return 1;
		}

		int SetInterpolationAmount(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			float flInterpolationAmount = luaL_checknumber(L, 2);
			lchan->netchan->SetInterpolationAmount(flInterpolationAmount);
			return 0;
		}

		int SetRemoteFramerate(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			float flFrameTime = luaL_checknumber(L, 2);
			float flFrameTimeStdDeviation = luaL_checknumber(L, 3);
			lchan->netchan->SetRemoteFramerate(flFrameTime, flFrameTimeStdDeviation);
			return 0;
		}

		int SetMaxRoutablePayloadSize(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			int nSplitSize = luaL_checkinteger(L, 2);
			lchan->netchan->SetMaxRoutablePayloadSize(nSplitSize);
			return 0;
		}

		int GetMaxRoutablePayloadSize(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushinteger(L, lchan->netchan->GetMaxRoutablePayloadSize());
			return 1;
		}

		int GetProtocolVersion(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				luaL_error(L, "Argument #1: Expected NetChannel, received nil");
				return 0;
			}

			LuaNetChannel* lchan = static_cast<LuaNetChannel*>(luaL_checkudata(L, 1, "NetChannel"));
			if (lchan->netchan == nullptr)
			{
				luaL_error(L, "NetChannel is nullptr");
				return 0;
			}

			lua_pushinteger(L, lchan->netchan->GetProtocolVersion());
			return 1;
		}
	}
}