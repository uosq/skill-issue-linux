#include "ipanel.h"

typedef struct wrect_s
{
	int	left;
	int right;
	int top;
	int bottom;
} wrect_t;

class CHudTexture
{
public:
	void* vftp;
	char szShortName[64];
	char szTextureFile[64];
	bool bRenderUsingFont;
	bool bPrecached;
	char cCharacterInFont;
	unsigned long hFont;
	int textureId;
	float texCoords[4];
	wrect_t rc;

public:
	int Width() const
	{
		return rc.right - rc.left;
	}
	int Height() const
	{
		return rc.bottom - rc.top;
	}
};