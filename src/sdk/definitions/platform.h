//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//

#pragma once

#include "Steam/SteamTypes.h"
#include <cassert>

#include <stdint.h>
#include "types.h"

#include <malloc.h>
#include <new>

// need this for memset
#include <string.h>

#define IsRetail() false
#define IsRelease() true
#define IsDebug() false

// Deprecating, infavor of IsX360() which will revert to IsXbox()
// after confidence of xbox 1 code flush
#define IsXbox()	false

#define IsPC() true
#define IsWindows() false
#define IsConsole() false
#define IsX360() false
#define IsPS3() false
#define IsLinux() true
#define IsOSX() false
	
#define IsPosix() true
#define IsPlatformOpenGL() true

typedef unsigned char uint8;
typedef signed char int8;

	// Avoid redefinition warnings if a previous header defines this.
	#undef OVERRIDE
	#if __cplusplus >= 201103L
		#define OVERRIDE override
		#if defined(__clang__)
			// warning: 'override' keyword is a C++11 extension [-Wc++11-extensions]
			// Disabling this warning is less intrusive than enabling C++11 extensions
			#pragma GCC diagnostic ignored "-Wc++11-extensions"
		#endif
	#else
		#define OVERRIDE
	#endif

//-----------------------------------------------------------------------------
// Set up platform type defines.
//-----------------------------------------------------------------------------

#define IsPC()			true
#define IsGameConsole()	false

#define IsPlatform64Bits()	true

// From steam/steamtypes.h
// RTime32
// We use this 32 bit time representing real world time.
// It offers 1 second resolution beginning on January 1, 1970 (Unix time)
typedef uint32_t RTime32;

typedef float				float32;
typedef double				float64;

// for when we don't care about how many bits we use
typedef unsigned int		uint;

// This can be used to ensure the size of pointers to members when declaring
// a pointer type for a class that has only been forward declared
#ifdef _MSC_VER
#define SINGLE_INHERITANCE __single_inheritance
#define MULTIPLE_INHERITANCE __multiple_inheritance
#else
#define SINGLE_INHERITANCE
#define MULTIPLE_INHERITANCE
#endif

#define NO_VTABLE

#define NORETURN

// This can be used to declare an abstract (interface only) class.
// Classes marked abstract should not be instantiated.  If they are, and access violation will occur.
//
// Example of use:
//
// abstract_class CFoo
// {
//      ...
// }
//
// MSDN __declspec(novtable) documentation: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_langref_novtable.asp
//
// Note: NJS: This is not enabled for regular PC, due to not knowing the implications of exporting a class with no no vtable.
//       It's probable that this shouldn't be an issue, but an experiment should be done to verify this.
//
#define abstract_class class


// MSVC CRT uses 0x7fff while gcc uses MAX_INT, leading to mismatches between platforms
// As a result, we pick the least common denominator here.  This should be used anywhere
// you might typically want to use RAND_MAX
#define VALVE_RAND_MAX 0x7fff

/*
FIXME: Enable this when we no longer fear change =)

// need these for the limits
#include <limits.h>
#include <float.h>

// Maximum and minimum representable values
#define  INT8_MAX			SCHAR_MAX
#define  INT16_MAX			SHRT_MAX
#define  INT32_MAX			LONG_MAX
#define  INT64_MAX			(((int64)~0) >> 1)

#define  INT8_MIN			SCHAR_MIN
#define  INT16_MIN			SHRT_MIN
#define  INT32_MIN			LONG_MIN
#define  INT64_MIN			(((int64)1) << 63)

#define  UINT8_MAX			((uint8)~0)
#define  UINT16_MAX			((uint16)~0)
#define  UINT32_MAX			((uint32_t)~0)
#define  UINT64_MAX			((uint64_t)~0)

#define  UINT8_MIN			0
#define  UINT16_MIN			0
#define  UINT32_MIN			0
#define  UINT64_MIN			0

#ifndef  UINT_MIN
#define  UINT_MIN			UINT32_MIN
#endif

#define  FLOAT32_MAX		FLT_MAX
#define  FLOAT64_MAX		DBL_MAX

#define  FLOAT32_MIN FLT_MIN
#define  FLOAT64_MIN DBL_MIN
*/

// portability / compiler settings
#if defined(_WIN32) && !defined(WINDED)

#if defined(_M_IX86)
#define __i386__	1
#endif

#elif POSIX
#if defined( OSX ) && defined( CARBON_WORKAROUND )
#define DWORD unsigned int
#else
typedef unsigned int DWORD;
#endif
typedef unsigned short WORD;
typedef void * HINSTANCE;
#define _MAX_PATH PATH_MAX
#define __cdecl
#define __stdcall
#define __declspec

#endif // defined(_WIN32) && !defined(WINDED)

#define MAX_FILEPATH 512 

// Defines MAX_PATH
#ifndef MAX_PATH
#define MAX_PATH  260
#endif

#ifdef _WIN32
#define MAX_UNICODE_PATH 32767
#else
#define MAX_UNICODE_PATH MAX_PATH
#endif

#define MAX_UNICODE_PATH_IN_UTF8 MAX_UNICODE_PATH*4

#if !defined( offsetof )
	#ifdef __GNUC__
		#define offsetof( type, var ) __builtin_offsetof( type, var )
	#else
		#define offsetof(s,m)	(size_t)&(((s *)0)->m)
	#endif
#endif // !defined( offsetof )


#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) //  need macro for constant expression

// Used to step into the debugger
#if defined( _WIN32 ) && !defined( _X360 )
#define DebuggerBreak()  __debugbreak()
#elif defined( _X360 )
#define DebuggerBreak() DebugBreak()
#else
	// On OSX, SIGTRAP doesn't really stop the thread cold when debugging.
	// So if being debugged, use INT3 which is precise.
#ifdef OSX
#define DebuggerBreak()  if ( Plat_IsInDebugSession() ) { __asm ( "int $3" ); } else { raise(SIGTRAP); }
#else
#define DebuggerBreak()  raise(SIGTRAP)
#endif
#endif
#define	DebuggerBreakIfDebugging() if ( !Plat_IsInDebugSession() ) ; else DebuggerBreak()

#ifdef STAGING_ONLY
#define	DebuggerBreakIfDebugging_StagingOnly() if ( !Plat_IsInDebugSession() ) ; else DebuggerBreak()
#else
#define	DebuggerBreakIfDebugging_StagingOnly()
#endif

// Allows you to specify code that should only execute if we are in a staging build. Otherwise the code noops.
#ifdef STAGING_ONLY
#define STAGING_ONLY_EXEC( _exec ) do { _exec; } while (0)
#else
#define STAGING_ONLY_EXEC( _exec ) do { } while (0)
#endif

// C functions for external declarations that call the appropriate C++ methods
#ifndef EXPORT
	#ifdef _WIN32
		#define EXPORT	_declspec( dllexport )
	#else
		#define EXPORT	/* */
	#endif
#endif

#if defined __i386__ && !defined __linux__
	#define id386	1
#else
	#define id386	0
#endif  // __i386__

// decls for aligning data
#ifdef _WIN32
        #define DECL_ALIGN(x) __declspec(align(x))

#elif GNUC
	#define DECL_ALIGN(x) __attribute__((aligned(x)))
#else
        #define DECL_ALIGN(x) /* */
#endif

//-----------------------------------------------------------------------------
// Convert int<-->pointer, avoiding 32/64-bit compiler warnings:
//-----------------------------------------------------------------------------
#define INT_TO_POINTER( i ) (void *)( ( i ) + (char *)NULL )
#define POINTER_TO_INT( p ) ( (int)(uintp)( p ) )


//-----------------------------------------------------------------------------
// Stack-based allocation related helpers
//-----------------------------------------------------------------------------
#if defined( GNUC )
	#define stackalloc( _size )		alloca( ALIGN_VALUE( _size, 16 ) )
#ifdef _LINUX
	#define mallocsize( _p )	( malloc_usable_size( _p ) )
#elif defined(OSX)
	#define mallocsize( _p )	( malloc_size( _p ) )
#else
#error
#endif
#elif defined ( _WIN32 )
	#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )
	#define mallocsize( _p )		( _msize( _p ) )
#endif

#define  stackfree( _p )			0

// Linux had a few areas where it didn't construct objects in the same order that Windows does.
// So when CVProfile::CVProfile() would access g_pMemAlloc, it would crash because the allocator wasn't initalized yet.
#ifdef POSIX
	#define CONSTRUCT_EARLY __attribute__((init_priority(101)))
#else
	#define CONSTRUCT_EARLY
	#endif

#if defined(_MSC_VER)
	#define SELECTANY __declspec(selectany)
	#define RESTRICT __restrict
	#define RESTRICT_FUNC __declspec(restrict)
	#define FMTFUNCTION( a, b )
#elif defined(GNUC)
	#define SELECTANY __attribute__((weak))
	#if defined(LINUX) && !defined(DEDICATED)
		#define RESTRICT
	#else
		#define RESTRICT __restrict
	#endif
	#define RESTRICT_FUNC
	// squirrel.h does a #define printf DevMsg which leads to warnings when we try
	// to use printf as the prototype format function. Using __printf__ instead.
	#define FMTFUNCTION( fmtargnumber, firstvarargnumber ) __attribute__ (( format( __printf__, fmtargnumber, firstvarargnumber )))
#else
	#define SELECTANY static
	#define RESTRICT
	#define RESTRICT_FUNC
	#define FMTFUNCTION( a, b )
#endif

// Used for standard calling conventions
#if defined( _WIN32 ) && !defined( _X360 )
	#define  STDCALL				__stdcall
	#define  FASTCALL				__fastcall
	#define  FORCEINLINE			__forceinline
	// GCC 3.4.1 has a bug in supporting forced inline of templated functions
	// this macro lets us not force inlining in that case
	#define  FORCEINLINE_TEMPLATE		__forceinline
#elif defined( _X360 )
	#define  STDCALL				__stdcall
	#ifdef FORCEINLINE
		#undef FORCEINLINE
#endif 
	#define  FORCEINLINE			__forceinline
	#define  FORCEINLINE_TEMPLATE		__forceinline
	#else
		#define  STDCALL
	#define  FASTCALL
	#ifdef _LINUX_DEBUGGABLE
		#define  FORCEINLINE
	#else
			#define  FORCEINLINE inline __attribute__ ((always_inline))
		#endif
	// GCC 3.4.1 has a bug in supporting forced inline of templated functions
	// this macro lets us not force inlining in that case
	#define FORCEINLINE_TEMPLATE	inline
//	#define  __stdcall			__attribute__ ((__stdcall__))
#endif

// Force a function call site -not- to inlined. (useful for profiling)
#define DONT_INLINE(a) (((int)(a)+1)?(a):(a))

// Pass hints to the compiler to prevent it from generating unnessecary / stupid code
// in certain situations.  Several compilers other than MSVC also have an equivilent
// construct.
//
// Essentially the 'Hint' is that the condition specified is assumed to be true at
// that point in the compilation.  If '0' is passed, then the compiler assumes that
// any subsequent code in the same 'basic block' is unreachable, and thus usually
// removed.
#ifdef _MSC_VER
	#define HINT(THE_HINT)	__assume((THE_HINT))
#else
	#define HINT(THE_HINT)	0
#endif

// Marks the codepath from here until the next branch entry point as unreachable,
// and asserts if any attempt is made to execute it.
#define UNREACHABLE() { Assert(0); HINT(0); }

// In cases where no default is present or appropriate, this causes MSVC to generate
// as little code as possible, and throw an assertion in debug.
#define NO_DEFAULT default: UNREACHABLE();


#ifdef _WIN32

// Remove warnings from warning level 4.
#pragma warning(disable : 4514) // warning C4514: 'acosl' : unreferenced inline function has been removed
#pragma warning(disable : 4100) // warning C4100: 'hwnd' : unreferenced formal parameter
#pragma warning(disable : 4127) // warning C4127: conditional expression is constant
#pragma warning(disable : 4512) // warning C4512: 'InFileRIFF' : assignment operator could not be generated
#pragma warning(disable : 4611) // warning C4611: interaction between '_setjmp' and C++ object destruction is non-portable
#pragma warning(disable : 4710) // warning C4710: function 'x' not inlined
#pragma warning(disable : 4702) // warning C4702: unreachable code
#pragma warning(disable : 4505) // unreferenced local function has been removed
#pragma warning(disable : 4239) // nonstandard extension used : 'argument' ( conversion from class Vector to class Vector& )
#pragma warning(disable : 4097) // typedef-name 'BaseClass' used as synonym for class-name 'CFlexCycler::CBaseFlex'
#pragma warning(disable : 4324) // Padding was added at the end of a structure
#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4305)	// truncation from 'const double ' to 'float '
#pragma warning(disable : 4786)	// Disable warnings about long symbol names
#pragma warning(disable : 4250) // 'X' : inherits 'Y::Z' via dominance
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4481) // warning C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable : 4748) // warning C4748: /GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function

#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	// Disable warnings about private copy constructors
#pragma warning(disable : 4121)	// warning C4121: 'symbol' : alignment of a member was sensitive to packing
#pragma warning(disable : 4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc (disabled due to std headers having exception syntax)
#endif

#if _MSC_VER >= 1400
#pragma warning(disable : 4996)	// functions declared deprecated
#endif


#endif // _WIN32

#if defined( LINUX ) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
  // based on some Jonathan Wakely macros on the net...
  #define GCC_DIAG_STR(s) #s
  #define GCC_DIAG_JOINSTR(x,y) GCC_DIAG_STR(x ## y)
  #define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
  #define GCC_DIAG_PRAGMA(x)	GCC_DIAG_DO_PRAGMA(GCC diagnostic x)

  #define GCC_DIAG_PUSH_OFF(x)	GCC_DIAG_PRAGMA(push) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
  #define GCC_DIAG_POP()		GCC_DIAG_PRAGMA(pop)
#else
  #define GCC_DIAG_PUSH_OFF(x)
  #define GCC_DIAG_POP()
#endif

#ifdef LINUX
#pragma GCC diagnostic ignored "-Wconversion-null"	// passing NULL to non-pointer argument 1
#pragma GCC diagnostic ignored "-Wpointer-arith"	// NULL used in arithmetic. Ie, vpanel == NULL where VPANEL is uint.
#pragma GCC diagnostic ignored "-Wswitch"				// enumeration values not handled in switch
#endif

#ifdef OSX
#pragma GCC diagnostic ignored "-Wconversion-null"			// passing NULL to non-pointer argument 1
#pragma GCC diagnostic ignored "-Wnull-arithmetic"			// NULL used in arithmetic. Ie, vpanel == NULL where VPANEL is uint.
#pragma GCC diagnostic ignored "-Wswitch-enum"				// enumeration values not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"					// enumeration values not handled in switch
#endif


// When we port to 64 bit, we'll have to resolve the int, ptr vs size_t 32/64 bit problems...
#if !defined( _WIN64 )
#pragma warning( disable : 4267 )	// conversion from 'size_t' to 'int', possible loss of data
#pragma warning( disable : 4311 )	// pointer truncation from 'char *' to 'int'
#pragma warning( disable : 4312 )	// conversion from 'unsigned int' to 'memhandle_t' of greater size
#endif


#define _stricmp stricmp
#define strcmpi stricmp
#define stricmp strcasecmp
#define _vsnprintf vsnprintf
#define _alloca alloca
#define _snprintf snprintf
#define GetProcAddress dlsym
#define _chdir chdir
#define _strnicmp strnicmp
#define strnicmp strncasecmp
#define _getcwd getcwd
#define _snwprintf swprintf
#define swprintf_s swprintf
#define wcsicmp _wcsicmp
#define _wcsicmp wcscmp
#define _finite finite
#define _tempnam tempnam
#define _unlink unlink
#define _access access
#define _mkdir(dir) mkdir( dir, S_IRWXU | S_IRWXG | S_IRWXO )
#define _wtoi(arg) wcstol(arg, NULL, 10)
#define _wtoi64(arg) wcstoll(arg, NULL, 10)


//-----------------------------------------------------------------------------
// FP exception handling
//-----------------------------------------------------------------------------
//#define CHECK_FLOAT_EXCEPTIONS		1

//-----------------------------------------------------------------------------
// Purpose: Standard functions for handling endian-ness
//-----------------------------------------------------------------------------

//-------------------------------------
// Basic swaps
//-------------------------------------

template <typename T>
inline T WordSwapC( T w )
{
   uint16 temp;

   temp  = ((*((uint16 *)&w) & 0xff00) >> 8);
   temp |= ((*((uint16 *)&w) & 0x00ff) << 8);

   return *((T*)&temp);
}

template <typename T>
inline T DWordSwapC( T dw )
{
   uint32_t temp;

   temp  =   *((uint32_t *)&dw) 				>> 24;
   temp |= ((*((uint32_t *)&dw) & 0x00FF0000) >> 8);
   temp |= ((*((uint32_t *)&dw) & 0x0000FF00) << 8);
   temp |= ((*((uint32_t *)&dw) & 0x000000FF) << 24);

   return *((T*)&temp);
}

template <typename T>
inline T QWordSwapC( T dw )
{
	// Assert sizes passed to this are already correct, otherwise
	// the cast to uint64_t * below is unsafe and may have wrong results 
	// or even crash.
	assert( sizeof( dw ) == sizeof(uint64_t) );

	uint64_t temp;

	temp  =   *((uint64_t *)&dw) 				         >> 56;
	temp |= ((*((uint64_t *)&dw) & 0x00FF000000000000ull) >> 40);
	temp |= ((*((uint64_t *)&dw) & 0x0000FF0000000000ull) >> 24);
	temp |= ((*((uint64_t *)&dw) & 0x000000FF00000000ull) >> 8);
	temp |= ((*((uint64_t *)&dw) & 0x00000000FF000000ull) << 8);
	temp |= ((*((uint64_t *)&dw) & 0x0000000000FF0000ull) << 24);
	temp |= ((*((uint64_t *)&dw) & 0x000000000000FF00ull) << 40);
	temp |= ((*((uint64_t *)&dw) & 0x00000000000000FFull) << 56);

	return *((T*)&temp);
}

//-------------------------------------
// Fast swaps
//-------------------------------------

#define WordSwap  WordSwapC
#define DWordSwap DWordSwapC

// No ASM implementation for this yet
#define QWordSwap QWordSwapC

//-------------------------------------
// The typically used methods.
//-------------------------------------

#if defined(__i386__) && !defined(VALVE_LITTLE_ENDIAN)
#define VALVE_LITTLE_ENDIAN 1
#endif

#if defined( _SGI_SOURCE ) || defined( _X360 )
#define	VALVE_BIG_ENDIAN 1
#endif

// If a swapped float passes through the fpu, the bytes may get changed.
// Prevent this by swapping floats as DWORDs.
#define SafeSwapFloat( pOut, pIn )	(*((uint*)pOut) = DWordSwap( *((uint*)pIn) ))

// @Note (toml 05-02-02): this technique expects the compiler to
// optimize the expression and eliminate the other path. On any new
// platform/compiler this should be tested.
inline short BigShort( short val )		{ int test = 1; return ( *(char *)&test == 1 ) ? WordSwap( val )  : val; }
inline uint16 BigWord( uint16 val )		{ int test = 1; return ( *(char *)&test == 1 ) ? WordSwap( val )  : val; }
inline long BigLong( long val )			{ int test = 1; return ( *(char *)&test == 1 ) ? DWordSwap( val ) : val; }
inline uint32_t BigDWord( uint32_t val )	{ int test = 1; return ( *(char *)&test == 1 ) ? DWordSwap( val ) : val; }
inline short LittleShort( short val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : WordSwap( val ); }
inline uint16 LittleWord( uint16 val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : WordSwap( val ); }
inline long LittleLong( long val )		{ int test = 1; return ( *(char *)&test == 1 ) ? val : DWordSwap( val ); }
inline uint32_t LittleDWord( uint32_t val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : DWordSwap( val ); }
inline uint64_t LittleQWord( uint64_t val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : QWordSwap( val ); }
inline short SwapShort( short val )					{ return WordSwap( val ); }
inline uint16 SwapWord( uint16 val )				{ return WordSwap( val ); }
inline long SwapLong( long val )					{ return DWordSwap( val ); }
inline uint32_t SwapDWord( uint32_t val )				{ return DWordSwap( val ); }

// Pass floats by pointer for swapping to avoid truncation in the fpu
inline void BigFloat( float *pOut, const float *pIn )		{ int test = 1; ( *(char *)&test == 1 ) ? SafeSwapFloat( pOut, pIn ) : ( *pOut = *pIn ); }
inline void LittleFloat( float *pOut, const float *pIn )	{ int test = 1; ( *(char *)&test == 1 ) ? ( *pOut = *pIn ) : SafeSwapFloat( pOut, pIn ); }
inline void SwapFloat( float *pOut, const float *pIn )		{ SafeSwapFloat( pOut, pIn ); }

FORCEINLINE uint32_t LoadLittleDWord( const uint32_t *base, unsigned int dwordIndex )
	{
		return LittleDWord( base[dwordIndex] );
	}

FORCEINLINE void StoreLittleDWord( uint32_t *base, unsigned int dwordIndex, uint32_t dword )
	{
		base[dwordIndex] = LittleDWord(dword);
	}






#if defined( _WIN32 ) && defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	extern "C" unsigned __int64 __rdtsc();
	#pragma intrinsic(__rdtsc)
#endif

inline uint64_t Plat_Rdtsc()
{
#if defined( _X360 )
	return ( uint64_t )__mftb32();
#elif defined( _WIN64 )
	return ( uint64_t )__rdtsc();
#elif defined( _WIN32 )
  #if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	return ( uint64_t )__rdtsc();
  #else
    __asm rdtsc;
	__asm ret;
  #endif
#elif defined( __i386__ )
	uint64_t val;
	__asm__ __volatile__ ( "rdtsc" : "=A" (val) );
	return val;
#elif defined( __x86_64__ )
	uint32_t lo, hi;
	__asm__ __volatile__ ( "rdtsc" : "=a" (lo), "=d" (hi));
	return ( ( ( uint64_t )hi ) << 32 ) | lo;
#else
	#error
#endif
}

// b/w compatibility
#define Sys_FloatTime Plat_FloatTime

// Protect against bad auto operator=
#define DISALLOW_OPERATOR_EQUAL( _classname )			\
	private:											\
		_classname &operator=( const _classname & );	\
	public:

// Define a reasonable operator=
#define IMPLEMENT_OPERATOR_EQUAL( _classname )			\
	public:												\
		_classname &operator=( const _classname &src )	\
		{												\
			memcpy( this, &src, sizeof(_classname) );	\
			return *this;								\
		}

// Processor Information:
struct CPUInformation
{
	int	 m_Size;		// Size of this structure, for forward compatability.

	bool m_bRDTSC : 1,	// Is RDTSC supported?
		 m_bCMOV  : 1,  // Is CMOV supported?
		 m_bFCMOV : 1,  // Is FCMOV supported?
		 m_bSSE	  : 1,	// Is SSE supported?
		 m_bSSE2  : 1,	// Is SSE2 Supported?
		 m_b3DNow : 1,	// Is 3DNow! Supported?
		 m_bMMX   : 1,	// Is MMX supported?
		 m_bHT	  : 1;	// Is HyperThreading supported?

	uint8 m_nLogicalProcessors;		// Number op logical processors.
	uint8 m_nPhysicalProcessors;	// Number of physical processors
	
	bool m_bSSE3 : 1,
		 m_bSSSE3 : 1,
		 m_bSSE4a : 1,
		 m_bSSE41 : 1,
		 m_bSSE42 : 1;	

	int64 m_Speed;						// In cycles per second.

	char* m_szProcessorID;				// Processor vendor Identification.

	uint32_t m_nModel;
	uint32_t m_nFeatures[3];

	CPUInformation(): m_Size(0){}
};

// Have to return a pointer, not a reference, because references are not compatible with the



#define MEMORY_INFORMATION_VERSION 0

struct MemoryInformation
{
	int m_nStructVersion;

	uint m_nPhysicalRamMbTotal;
	uint m_nPhysicalRamMbAvailable;
	
	uint m_nVirtualRamMbTotal;
	uint m_nVirtualRamMbAvailable;

	inline MemoryInformation()
	{
		memset( this, 0, sizeof( *this ) );
		m_nStructVersion = MEMORY_INFORMATION_VERSION;
	}
};

// Returns true if the passed in MemoryInformation structure was filled out, otherwise false.






// ---------------------------------------------------------------------------------- //
// Performance Monitoring Events - L2 stats etc...
// ---------------------------------------------------------------------------------- //



//-----------------------------------------------------------------------------
// Thread related functions
//-----------------------------------------------------------------------------

// Sets a hardware data breakpoint on the given address. Currently Win32-only.
// Specify 1, 2, or 4 bytes for nWatchBytes; pass 0 to unregister the address.


// Apply current hardware data breakpoints to a newly created thread.


//-----------------------------------------------------------------------------
// Process related functions
//-----------------------------------------------------------------------------

#ifndef _WIN32
// helper function for OS's that don't have a ::GetCommandLine() call

#endif


//-----------------------------------------------------------------------------
// Security related functions
//-----------------------------------------------------------------------------
// Ensure that the hardware key's drivers have been installed.


// Ok, so this isn't a very secure way to verify the hardware key for now.  It
// is primarially depending on the fact that all the binaries have been wrapped
// with the secure wrapper provided by the hardware keys vendor.


// The same as above, but notifies user with a message box when the key isn't in
// and gives him an opportunity to correct the situation.


// Can be called in real time, doesn't perform the verify every frame.  Mainly just
// here to allow the game to drop out quickly when the key is removed, rather than
// allowing the wrapper to pop up it's own blocking dialog, which the engine doesn't
// like much.


//-----------------------------------------------------------------------------
// Just logs file and line to simple.log
//-----------------------------------------------------------------------------


#if _X360
#define Plat_FastMemset XMemSet
#define Plat_FastMemcpy XMemCpy
#else
#define Plat_FastMemset memset
#define Plat_FastMemcpy memcpy
#endif

//-----------------------------------------------------------------------------
// Returns true if debugger attached, false otherwise
//-----------------------------------------------------------------------------
#if defined(_WIN32) || defined(LINUX) || defined(OSX)


#else
inline bool Plat_IsInDebugSession( bool bForceRecheck = false ) { return false; }
#define Plat_DebugString(s) ((void)0)
#endif

//-----------------------------------------------------------------------------
// Returns true if running on a 64 bit (windows) OS
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// XBOX Components valid in PC compilation space
//-----------------------------------------------------------------------------

#define XBOX_DVD_SECTORSIZE			2048
#define XBOX_DVD_ECC_SIZE			32768 // driver reads in quantum ECC blocks
#define XBOX_HDD_SECTORSIZE			512

// Custom windows messages for Xbox input
#define WM_XREMOTECOMMAND					(WM_USER + 100)
#define WM_XCONTROLLER_KEY					(WM_USER + 101)
#define WM_SYS_UI							(WM_USER + 102)
#define WM_SYS_SIGNINCHANGED				(WM_USER + 103)
#define WM_SYS_STORAGEDEVICESCHANGED		(WM_USER + 104)
#define WM_SYS_PROFILESETTINGCHANGED		(WM_USER + 105)
#define WM_SYS_MUTELISTCHANGED				(WM_USER + 106)
#define WM_SYS_INPUTDEVICESCHANGED			(WM_USER + 107)
#define WM_SYS_INPUTDEVICECONFIGCHANGED		(WM_USER + 108)
#define WM_LIVE_CONNECTIONCHANGED			(WM_USER + 109)
#define WM_LIVE_INVITE_ACCEPTED				(WM_USER + 110)
#define WM_LIVE_LINK_STATE_CHANGED			(WM_USER + 111)
#define WM_LIVE_CONTENT_INSTALLED			(WM_USER + 112)
#define WM_LIVE_MEMBERSHIP_PURCHASED		(WM_USER + 113)
#define WM_LIVE_VOICECHAT_AWAY				(WM_USER + 114)
#define WM_LIVE_PRESENCE_CHANGED			(WM_USER + 115)
#define WM_FRIENDS_PRESENCE_CHANGED			(WM_USER + 116)
#define WM_FRIENDS_FRIEND_ADDED				(WM_USER + 117)
#define WM_FRIENDS_FRIEND_REMOVED			(WM_USER + 118)
#define WM_CUSTOM_GAMEBANNERPRESSED			(WM_USER + 119)
#define WM_CUSTOM_ACTIONPRESSED				(WM_USER + 120)
#define WM_XMP_STATECHANGED					(WM_USER + 121)
#define WM_XMP_PLAYBACKBEHAVIORCHANGED		(WM_USER + 122)
#define WM_XMP_PLAYBACKCONTROLLERCHANGED	(WM_USER + 123)

// flat view, 6 hw threads
#define XBOX_PROCESSOR_0			( 1<<0 )
#define XBOX_PROCESSOR_1			( 1<<1 )
#define XBOX_PROCESSOR_2			( 1<<2 )
#define XBOX_PROCESSOR_3			( 1<<3 )
#define XBOX_PROCESSOR_4			( 1<<4 )
#define XBOX_PROCESSOR_5			( 1<<5 )

// core view, 3 cores with 2 hw threads each
#define XBOX_CORE_0_HWTHREAD_0		XBOX_PROCESSOR_0
#define XBOX_CORE_0_HWTHREAD_1		XBOX_PROCESSOR_1
#define XBOX_CORE_1_HWTHREAD_0		XBOX_PROCESSOR_2
#define XBOX_CORE_1_HWTHREAD_1		XBOX_PROCESSOR_3
#define XBOX_CORE_2_HWTHREAD_0		XBOX_PROCESSOR_4
#define XBOX_CORE_2_HWTHREAD_1		XBOX_PROCESSOR_5
//
// GET_OUTER()
//
// A platform-independent way for a contained class to get a pointer to its
// owner. If you know a class is exclusively used in the context of some
// "outer" class, this is a much more space efficient way to get at the outer
// class than having the inner class store a pointer to it.
//
//	class COuter
//	{
//		class CInner // Note: this does not need to be a nested class to work
//		{
//			void PrintAddressOfOuter()
//			{
//				printf( "Outer is at 0x%x\n", GET_OUTER( COuter, m_Inner ) );
//			}
//		};
//
//		CInner m_Inner;
//		friend class CInner;
//	};