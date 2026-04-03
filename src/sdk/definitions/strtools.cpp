#include "strtools.h"
#include <cassert>
#include <cstdio>
#include <cstring>

//-----------------------------------------------------------------------------
// Purpose: Returns the 4 bit nibble for a hex character
// Input  : c -
// Output : unsigned char
//-----------------------------------------------------------------------------
unsigned char V_nibble(char c)
{
	if ((c >= '0') && (c <= '9'))
	{
		return (unsigned char)(c - '0');
	}

	if ((c >= 'A') && (c <= 'F'))
	{
		return (unsigned char)(c - 'A' + 0x0a);
	}

	if ((c >= 'a') && (c <= 'f'))
	{
		return (unsigned char)(c - 'a' + 0x0a);
	}

	return '0';
}

void V_binarytohex(const byte *in, int inputbytes, char *out, int outsize)
{
	assert(outsize >= 1);
	char doublet[10];
	int i;

	out[0] = 0;

	for (i = 0; i < inputbytes; i++)
	{
		unsigned char c = in[i];
		snprintf(doublet, sizeof(doublet), "%02x", c);
		strncat(out, doublet, outsize);
	}
}
