#include "draw.h"

#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

static void Line(int x0, int y0, int x1, int y1)
{
	interfaces::Surface->DrawLine(x0, y0, x1, y1);
}

static void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	interfaces::Surface->DrawSetColor(r, g, b, a);
	interfaces::Surface->DrawSetTextColor(r, g, b, a);
}

static void GetTextSize(uint64_t font, const std::string &text, int &w, int &h)
{
	// this is shit syntax
	std::wstring wtext(text.begin(), text.end());
	interfaces::Surface->GetTextSize(font, wtext.c_str(), w, h);
}

static void GetCursorPos(int &x, int &y)
{
	interfaces::Surface->SurfaceGetCursorPos(x, y);
}

static void SetCursorPos(int x, int y)
{
	interfaces::Surface->SurfaceSetCursorPos(x, y);
}

static void Circle(int x, int y, int radius, int segments)
{
	interfaces::Surface->DrawOutlinedCircle(x, y, radius, segments);
}

static bool HasFocus()
{
	return interfaces::Surface->HasFocus();
}

static void GetScreenSize(int &w, int &h)
{
	interfaces::Surface->GetScreenSize(w, h);
}

static void Text(int x, int y, const std::string &text)
{
	std::wstring wtext(text.begin(), text.end()); // this is fucking stupid
	interfaces::Surface->DrawSetTextPos(x, y);
	interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());
}

uint64_t CreateFont(const std::string &name, int height, int weight, int flags)
{
	uint64_t font = interfaces::Surface->CreateFont();
	interfaces::Surface->SetFontGlyphSet(font, name.c_str(), height, weight, 0, 0, flags);
	return font;
}

static void SetFont(uint64_t font)
{
	interfaces::Surface->DrawSetTextFont(font);
}

static void Rect(int x, int y, int w, int h)
{
	interfaces::Surface->DrawFilledRect(x, y, x + w, y + h);
}

static void RectFade(int x0, int y0, int x1, int y1, int alpha0, int alpha1, bool bHorizontal)
{
	interfaces::Surface->DrawFilledRectFade(x0, y0, x1, y1, (unsigned int)alpha0, (unsigned int)alpha1, bHorizontal);
}

static bool IsCursorVisible()
{
	return interfaces::Surface->IsCursorVisible();
}

void Draw_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Draw");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_DRAW);
		engine->RegisterTypedef("uint64", "Font");
		engine->RegisterGlobalFunction("void Line(int x0, int y0, int x1, int y1)", asFUNCTION(Line), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetColor(uint8 r, uint8 g, uint8 b, uint8 a)", asFUNCTION(SetColor), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetTextSize(Font font, const string &in text, int &out w, int &out h)", asFUNCTION(GetTextSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetCursorPos(int &out x, int &out y)", asFUNCTION(GetCursorPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorPos(int x, int y)", asFUNCTION(SetCursorPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Circle(int x, int y, int radius, int segments)", asFUNCTION(Circle), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool HasFocus()", asFUNCTION(HasFocus), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetScreenSize(int &out w, int &out h)", asFUNCTION(GetScreenSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("Font CreateFont(const string &in name, int height, int weight, int flags)", asFUNCTION(CreateFont), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetFont(Font font)", asFUNCTION(SetFont), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Rect(int x, int y, int w, int h)", asFUNCTION(Rect), asCALL_CDECL);
		engine->RegisterGlobalFunction("void RectFade(int x0, int y0, int x1, int y1, int alpha0, int alpha1, bool bHorizontal)", asFUNCTION(RectFade), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsCursorVisible()", asFUNCTION(IsCursorVisible), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Text(int x, int y, const string &in text)", asFUNCTION(Text), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}