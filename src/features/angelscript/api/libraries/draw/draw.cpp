#include "draw.h"

#include "../../../../../sdk/FontManager/fontmanager.h"
#include "../../../../../sdk/helpers/draw/draw.h"
#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

void Line(int x0, int y0, int x1, int y1)
{
	interfaces::Surface->DrawLine(x0, y0, x1, y1);
}

void SetColor(int r, int g, int b, int a)
{
	interfaces::Surface->DrawSetColor(r, g, b, a);
	interfaces::Surface->DrawSetTextColor(r, g, b, a);
}

void GetTextSize(const std::string &text, int &w, int &h)
{
	helper::draw::GetTextSize(text, w, h);
}

void GetCursorPos(int &x, int &y)
{
	interfaces::Surface->SurfaceGetCursorPos(x, y);
}

void SetCursorPos(int x, int y)
{
	interfaces::Surface->SurfaceSetCursorPos(x, y);
}

void Circle(int x, int y, int radius, int segments)
{
	interfaces::Surface->DrawOutlinedCircle(x, y, radius, segments);
}

bool HasFocus()
{
	return interfaces::Surface->HasFocus();
}

void GetScreenSize(int &w, int &h)
{
	interfaces::Surface->GetScreenSize(w, h);
}

void Text(int x, int y, const std::string &text)
{
	std::wstring wtext(text.begin(),
			   text.end()); // this is fucking stupid
	interfaces::Surface->DrawSetTextPos(x, y);
	interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());
}

bool CreateFont(const std::string &name, const std::string &family, int height, int weight, int flags)
{
	int font = FontManager::CreateFont(name, family, height, weight, flags);
	return font != 0;
}

bool SetFont(const std::string &name)
{
	return FontManager::SetFont(name);
}

void Rect(int x, int y, int w, int h)
{
	interfaces::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void RectFade(int x0, int y0, int x1, int y1, int alpha0, int alpha1, bool bHorizontal)
{
	interfaces::Surface->DrawFilledRectFade(x0, y0, x1, y1, (unsigned int)alpha0, (unsigned int)alpha1,
						bHorizontal);
}

bool IsCursorVisible()
{
	return interfaces::Surface->IsCursorVisible();
}

void Draw_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Draw");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_DRAW);
		engine->RegisterGlobalFunction("void Line(int x0, int y0, int x1, int y1)", asFUNCTION(Line),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetColor(int r, int g, int b, int a)", asFUNCTION(SetColor),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetTextSize(const string &in text, int &out w, int "
					       "&out h)",
					       asFUNCTION(GetTextSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetCursorPos(int &out x, int &out y)", asFUNCTION(GetCursorPos),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorPos(int x, int y)", asFUNCTION(SetCursorPos),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void Circle(int x, int y, int radius, int segments)",
					       asFUNCTION(Circle), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool HasFocus()", asFUNCTION(HasFocus), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetScreenSize(int &out w, int &out h)", asFUNCTION(GetScreenSize),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool CreateFont(const string &in name, const string &in "
					       "family, int height, int weight, int flags)",
					       asFUNCTION(CreateFont), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool SetFont(const string &in name)", asFUNCTION(SetFont),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void Rect(int x, int y, int w, int h)", asFUNCTION(Rect), asCALL_CDECL);
		engine->RegisterGlobalFunction("void RectFade(int x0, int y0, int x1, int y1, int "
					       "alpha0, int alpha1, bool bHorizontal)",
					       asFUNCTION(RectFade), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsCursorVisible()", asFUNCTION(IsCursorVisible), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}