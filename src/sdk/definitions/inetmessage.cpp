#include "inetmessage.h"
#include "../interfaces/interfaces.h"
#include "bitbuf.h"
#include <cstdarg>

static char s_text[1024];

#define NET_TICK_SCALEUP	100000.0f

int Q_snprintf( char *pDest, int maxLen, char const *pFormat, ... )
{
	va_list marker;

	va_start( marker, pFormat );
	int len = vsnprintf( pDest, maxLen, pFormat, marker );
	va_end( marker );

	// Len < 0 represents an overflow
	if( len < 0 )
	{
		len = maxLen;
		pDest[maxLen-1] = 0;
	}

	return len;
}

bool NET_Tick::WriteToBuffer( bf_write &buffer )
{
	buffer.WriteUBitLong( GetType(), NETMSG_TYPE_BITS );
	buffer.WriteSBitLong( m_nTick, 32 );
	buffer.WriteUBitLong( std::clamp( (int)( NET_TICK_SCALEUP * m_flHostFrameTime ), 0, 65535 ), 16 );
	buffer.WriteUBitLong( std::clamp( (int)( NET_TICK_SCALEUP * m_flHostFrameTimeStdDeviation ), 0, 65535 ), 16 );
	return !buffer.IsOverflowed();

	/*using WriteToBufferFn = bool(*)(NET_Tick* self, bf_write& bf);
	static WriteToBufferFn original = reinterpret_cast<WriteToBufferFn>(sigscan_module("engine.so", "55 48 8D 0D C8 CA 00 00"));
	return(original)(this, buffer);*/
}

bool NET_Tick::ReadFromBuffer( bf_read &buffer )
{
	m_nTick = buffer.ReadLong();
	m_flHostFrameTime				= (float)buffer.ReadUBitLong( 16 ) / NET_TICK_SCALEUP;
	m_flHostFrameTimeStdDeviation	= (float)buffer.ReadUBitLong( 16 ) / NET_TICK_SCALEUP;
	return !buffer.IsOverflowed();

	/*using ReadToBufferFn = bool(*)(NET_Tick* self, bf_read& bf);
	static ReadToBufferFn original = reinterpret_cast<ReadToBufferFn>(sigscan_module("engine.so", "55 31 C0 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 89 F3 48 83 EC 28 4C 8B 2D ? ? ? ? 48 C7 45 B8 00 00 00 00 49 8B 7D 18 48 85 FF 74 ? 48 83 EC 08 45 31 C0 31 C9 48 8D 05 ? ? ? ? 31 D2 50 48 8D 05 ? ? ? ? 50 48 8D 05 ? ? ? ? 50 48 8D 05 ? ? ? ? 50 48 8D 75 B8 31 C0 68 16 04 00 00"));

	return original(this, buffer);*/
}

const char *NET_Tick::ToString(void) const
{
	Q_snprintf(s_text, sizeof(s_text), "%s: tick %i", GetName(), m_nTick );
	return s_text;
}

const char *CLC_Move::ToString(void) const
{
	return "My CLC_Move\n";
}

bool CLC_Move::WriteToBuffer( bf_write &buffer )
{
	buffer.WriteUBitLong( GetType(), NETMSG_TYPE_BITS );
	m_nLength = m_DataOut.GetNumBitsWritten();
	
	buffer.WriteUBitLong( m_nNewCommands, NUM_NEW_COMMAND_BITS );
	buffer.WriteUBitLong( m_nBackupCommands, NUM_BACKUP_COMMAND_BITS );
	
	buffer.WriteWord( m_nLength );	

	return buffer.WriteBits( m_DataOut.GetData(), m_nLength );

	/*using WriteToBufferFn = bool(*)(CLC_Move* self, bf_write& bf);
	static WriteToBufferFn original = reinterpret_cast<WriteToBufferFn>(sigscan_module("engine.so", "55 48 8D 0D 88 DD 00 00"));
	return original(this, buffer);*/
}

bool CLC_Move::ReadFromBuffer( bf_read &buffer )
{
	m_nNewCommands = buffer.ReadUBitLong( NUM_NEW_COMMAND_BITS );
	m_nBackupCommands = buffer.ReadUBitLong( NUM_BACKUP_COMMAND_BITS );
	m_nLength = buffer.ReadWord();
	m_DataIn = buffer;
	return buffer.SeekRelative( m_nLength );

	/*using ReadFromBufferFn = bool(*)(CLC_Move* self, bf_read& bf);
	static ReadFromBufferFn original = reinterpret_cast<ReadFromBufferFn>(sigscan_module("engine.so", "55 31 C0 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 53 48 89 F3 48 83 EC 28 4C 8B 25 ? ? ? ? 48 C7 45 B8 00 00 00 00 49 8B 7C 24 18 48 85 FF 74 ? 48 83 EC 08 45 31 C0 31 C9 48 8D 05 ? ? ? ? 31 D2 50 48 8D 05 ? ? ? ? 50 48 8D 05 ? ? ? ? 50 48 8D 05 ? ? ? ? 50 48 8D 75 B8 31 C0 6A 58"));
	return original(this, buffer);*/
}
