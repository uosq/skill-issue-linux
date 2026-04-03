#include "imgui.h"
#include <string>

#include "../../../../../imgui/imgui.h"
#include "../../../../../sdk/definitions/vector.h"
#include "../../../add_on/scriptarray/scriptarray.h"
#include "../../globals.h"

bool Begin(const std::string &name, bool &p_open, int flags = 0)
{
	return ImGui::Begin(name.c_str(), &p_open, flags);
}

bool BeginNoOpen(const std::string &name, int flags = 0)
{
	return ImGui::Begin(name.c_str(), nullptr, flags);
}

void End()
{
	ImGui::End();
}

bool BeginChildA(const std::string &str_id, const Vec2 &size = Vec2(0, 0), int child_flags = 0, int window_flags = 0)
{
	return ImGui::BeginChild(str_id.c_str(), ImVec2(size.x, size.y), (ImGuiChildFlags)child_flags,
				 (ImGuiWindowFlags)window_flags);
}

bool BeginChildB(unsigned int id, const Vec2 &size = Vec2(0, 0), int child_flags = 0, int window_flags = 0)
{
	return ImGui::BeginChild((ImGuiID)id, ImVec2(size.x, size.y), (ImGuiChildFlags)child_flags,
				 (ImGuiWindowFlags)window_flags);
}

void EndChild()
{
	return ImGui::EndChild();
}

bool IsWindowAppearing()
{
	return ImGui::IsWindowAppearing();
}

bool IsWindowCollapsed()
{
	return ImGui::IsWindowCollapsed();
}

bool IsWindowFocused(int flags = 0)
{
	return ImGui::IsWindowFocused(flags);
}

bool IsWindowHovered(int flags = 0)
{
	return ImGui::IsWindowHovered(flags);
}

Vec2 GetWindowPos()
{
	ImVec2 pos = ImGui::GetWindowPos();
	return {pos.x, pos.y};
}

Vec2 GetWindowSize()
{
	auto pos = ImGui::GetWindowSize();
	return {pos.x, pos.y};
}

float GetWindowWidth()
{
	return ImGui::GetWindowWidth();
}

float GetWindowHeight()
{
	return ImGui::GetWindowHeight();
}

void SetNextWindowPos(const Vec2 &pos, int cond = 0, const Vec2 &pivot = Vec2(0, 0))
{
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), cond, ImVec2(pivot.x, pivot.y));
}
void SetNextWindowSize(const Vec2 &size, int cond = 0)
{
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), cond);
}
void SetNextWindowContentSize(const Vec2 &size)
{
	ImGui::SetNextWindowContentSize(ImVec2(size.x, size.y));
}
void SetNextWindowCollapsed(bool collapsed, int cond = 0)
{
	ImGui::SetNextWindowCollapsed(collapsed, cond);
}
void SetNextWindowFocus()
{
	ImGui::SetNextWindowFocus();
}
void SetNextWindowScroll(const Vec2 &scroll)
{
	ImGui::SetNextWindowScroll(ImVec2(scroll.x, scroll.y));
}
void SetNextWindowBgAlpha(float alpha)
{
	ImGui::SetNextWindowBgAlpha(alpha);
}
void SetWindowPosA(const Vec2 &pos, int cond = 0)
{
	ImGui::SetWindowPos(ImVec2(pos.x, pos.y), cond);
}
void SetWindowSizeA(const Vec2 &size, int cond = 0)
{
	ImGui::SetWindowSize(ImVec2(size.x, size.y), cond);
}
void SetWindowCollapsedA(bool collapsed, int cond = 0)
{
	ImGui::SetWindowCollapsed(collapsed, cond);
}
void SetWindowFocusA()
{
	ImGui::SetWindowFocus();
}
void SetWindowPosB(const std::string &name, const Vec2 &pos, int cond = 0)
{
	ImGui::SetWindowPos(name.c_str(), ImVec2(pos.x, pos.y), cond);
}
void SetWindowSizeB(const std::string &name, const Vec2 &size, int cond = 0)
{
	ImGui::SetWindowSize(name.c_str(), ImVec2(size.x, size.y), cond);
}
void SetWindowCollapsedB(const std::string &name, bool collapsed, int cond = 0)
{
	ImGui::SetWindowCollapsed(name.c_str(), collapsed, cond);
}
void SetWindowFocusB(const std::string &name)
{
	ImGui::SetWindowFocus(name.c_str());
}

float GetScrollX()
{
	return ImGui::GetScrollX();
}
float GetScrollY()
{
	return ImGui::GetScrollY();
}
void SetScrollX(float scroll_x)
{
	return ImGui::SetScrollX(scroll_x);
}
void SetScrollY(float scroll_y)
{
	return ImGui::SetScrollY(scroll_y);
}
float GetScrollMaxX()
{
	return ImGui::GetScrollMaxX();
}
float GetScrollMaxY()
{
	return ImGui::GetScrollMaxY();
}
void SetScrollHereX(float center_x_ratio = 0.5f)
{
	return ImGui::SetScrollHereX(center_x_ratio);
}
void SetScrollHereY(float center_y_ratio = 0.5f)
{
	return ImGui::SetScrollHereY(center_y_ratio);
}
void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f)
{
	return ImGui::SetScrollFromPosX(local_x, center_x_ratio);
}
void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f)
{
	return ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

Vec2 GetFontTexUvWhitePixel()
{
	ImVec2 pos = ImGui::GetFontTexUvWhitePixel();
	return {pos.x, pos.y};
}
ImU32 GetColorU32A(ImGuiCol idx, float alpha_mul = 1.0f)
{
	return ImGui::GetColorU32(idx, alpha_mul);
}
ImU32 GetColorU32B(float r, float g, float b, float a)
{
	return ImGui::GetColorU32(ImVec4(r, g, b, a));
}
void GetStyleColorVec4(ImGuiCol idx, float &r, float &g, float &b, float &a)
{
	auto col = ImGui::GetStyleColorVec4(idx);
	r	 = col.x;
	g	 = col.y;
	b	 = col.z;
	a	 = col.w;
}

Vec2 GetCursorScreenPos()
{
	auto pos = ImGui::GetCursorScreenPos();
	return {pos.x, pos.y};
}
void SetCursorScreenPos(const Vec2 &pos)
{
	return ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y));
}
Vec2 GetContentRegionAvail()
{
	ImVec2 pos = ImGui::GetContentRegionAvail();
	return {pos.x, pos.y};
}
Vec2 GetCursorPos()
{
	ImVec2 pos = ImGui::GetCursorPos();
	return {pos.x, pos.y};
}
float GetCursorPosX()
{
	return ImGui::GetCursorPosX();
}
float GetCursorPosY()
{
	return ImGui::GetCursorPosY();
}
void SetCursorPos(const Vec2 &local_pos)
{
	return ImGui::SetCursorPos(ImVec2(local_pos.x, local_pos.y));
}
void SetCursorPosX(float local_x)
{
	return ImGui::SetCursorPosX(local_x);
}
void SetCursorPosY(float local_y)
{
	return ImGui::SetCursorPosY(local_y);
}
Vec2 GetCursorStartPos()
{
	ImVec2 pos = ImGui::GetCursorStartPos();
	return {pos.x, pos.y};
}

void Separator()
{
	return ImGui::Separator();
}
void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f)
{
	return ImGui::SameLine(offset_from_start_x, spacing);
}
void NewLine()
{
	return ImGui::NewLine();
}
void Spacing()
{
	return ImGui::Spacing();
}
void Dummy(const Vec2 &size)
{
	return ImGui::Dummy(ImVec2(size.x, size.y));
}
void Indent(float indent_w = 0.0f)
{
	return ImGui::Indent(indent_w);
}
void Unindent(float indent_w = 0.0f)
{
	return ImGui::Unindent(indent_w);
}
void BeginGroup()
{
	return ImGui::BeginGroup();
}
void EndGroup()
{
	return ImGui::EndGroup();
}
void AlignTextToFramePadding()
{
	return ImGui::AlignTextToFramePadding();
}
float GetTextLineHeight()
{
	return ImGui::GetTextLineHeight();
}
float GetTextLineHeightWithSpacing()
{
	return ImGui::GetTextLineHeightWithSpacing();
}
float GetFrameHeight()
{
	return ImGui::GetFrameHeight();
}
float GetFrameHeightWithSpacing()
{
	return ImGui::GetFrameHeightWithSpacing();
}

void TextUnformatted(const std::string &text)
{
	ImGui::TextUnformatted(text.c_str());
}
void TextColored(int r, int g, int b, int a, const std::string &fmt)
{
	ImGui::TextColored(ImVec4(r, g, b, a), "%s", fmt.c_str());
}
void TextDisabled(const std::string &fmt)
{
	ImGui::TextDisabled("%s", fmt.c_str());
}
void TextWrapped(const std::string &fmt)
{
	ImGui::TextWrapped("%s", fmt.c_str());
}
void LabelText(const std::string &label, const std::string &fmt)
{
	ImGui::LabelText(label.c_str(), "%s", fmt.c_str());
}
void BulletText(const std::string &fmt)
{
	ImGui::BulletText("%s", fmt.c_str());
}
void SeparatorText(const std::string &label)
{
	ImGui::SeparatorText(label.c_str());
}

bool Button(const std::string &label, const Vec2 &size = Vec2(0, 0))
{
	return ImGui::Button(label.c_str(), ImVec2(size.x, size.y));
}
bool SmallButton(const std::string &label)
{
	return ImGui::SmallButton(label.c_str());
}
bool InvisibleButton(const std::string &str_id, const Vec2 &size, int flags = 0)
{
	return ImGui::InvisibleButton(str_id.c_str(), ImVec2(size.x, size.y), flags);
}
bool ArrowButton(const std::string &str_id, int dir)
{
	return ImGui::ArrowButton(str_id.c_str(), (ImGuiDir)dir);
}
bool Checkbox(const std::string &label, bool &v)
{
	return ImGui::Checkbox(label.c_str(), &v);
}
bool CheckboxFlagsA(const std::string &label, int &flags, int flags_value)
{
	return ImGui::CheckboxFlags(label.c_str(), &flags, flags_value);
}
bool CheckboxFlagsB(const std::string &label, unsigned int &flags, unsigned int flags_value)
{
	return ImGui::CheckboxFlags(label.c_str(), &flags, flags_value);
}
bool RadioButtonA(const std::string &label, bool active)
{
	return ImGui::RadioButton(label.c_str(), active);
}
bool RadioButtonB(const std::string &label, int &v, int v_button)
{
	return ImGui::RadioButton(label.c_str(), &v, v_button);
}
void ProgressBarA(float fraction, const Vec2 &size_arg = Vec2(-FLT_MIN, 0))
{
	ImGui::ProgressBar(fraction, ImVec2(size_arg.x, size_arg.y), NULL);
}
void ProgressBarB(float fraction, const Vec2 &size_arg = Vec2(-FLT_MIN, 0), const std::string &overlay = "")
{
	ImGui::ProgressBar(fraction, ImVec2(size_arg.x, size_arg.y), overlay.c_str());
}
void Bullet()
{
	ImGui::Bullet();
}
bool TextLink(const std::string &label)
{
	return ImGui::TextLink(label.c_str());
}
bool TextLinkOpenURLA(const std::string &label)
{
	return ImGui::TextLinkOpenURL(label.c_str());
}
bool TextLinkOpenURLB(const std::string &label, const std::string &url)
{
	return ImGui::TextLinkOpenURL(label.c_str(), url.c_str());
}

bool BeginCombo(const std::string &label, const std::string &preview_value, int flags = 0)
{
	return ImGui::BeginCombo(label.c_str(), preview_value.c_str(), flags);
}
void EndCombo()
{
	return ImGui::EndCombo();
}
bool Combo(const std::string &label, int &current_item, CScriptArray *items, int popup_max_height_in_items)
{
	if (!items)
		return false;

	const size_t count = items->GetSize();

	std::vector<const char *> c_items;
	c_items.reserve(count);

	for (size_t i = 0; i < count; i++)
	{
		auto str = static_cast<std::string *>(items->At(i));
		c_items.push_back(str->c_str());
	}

	return ImGui::Combo(label.c_str(), &current_item, c_items.data(), static_cast<int>(c_items.size()),
			    popup_max_height_in_items);
}

void ImGui_RegisterLibrary(asIScriptEngine *engine)
{
	auto defNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("ImGui");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_IMGUI);
		engine->RegisterGlobalFunction("bool Begin(const string &in name, bool &out p_open, int "
					       "flags = 0)",
					       asFUNCTION(Begin), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Begin(const string &in name, int flags = 0)",
					       asFUNCTION(BeginNoOpen), asCALL_CDECL);
		engine->RegisterGlobalFunction("void End()", asFUNCTION(End), asCALL_CDECL);

		engine->RegisterGlobalFunction("bool BeginChild(const string &in str_id, const Vector2 "
					       "&in size = Vector2(0, 0), int child_flags = 0, int "
					       "window_flags = 0)",
					       asFUNCTION(BeginChildA), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool BeginChild(uint id, const Vector2 &in size = "
					       "Vector2(0, 0), int child_flags = 0, int window_flags = "
					       "0)",
					       asFUNCTION(BeginChildB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void EndChild()", asFUNCTION(EndChild), asCALL_CDECL);

		engine->RegisterGlobalFunction("bool IsWindowAppearing()", asFUNCTION(IsWindowAppearing), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsWindowCollapsed()", asFUNCTION(IsWindowCollapsed), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsWindowFocused(int flags = 0)", asFUNCTION(IsWindowFocused),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsWindowHovered(int flags = 0)", asFUNCTION(IsWindowHovered),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector2 GetWindowPos()", asFUNCTION(GetWindowPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector2 GetWindowSize()", asFUNCTION(GetWindowSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetWindowWidth()", asFUNCTION(GetWindowWidth), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetWindowHeight()", asFUNCTION(GetWindowHeight), asCALL_CDECL);

		engine->RegisterGlobalFunction("void SetNextWindowPos(const Vector2 &in pos, int cond = "
					       "0, const Vector2 &in pivot = Vector2(0, 0))",
					       asFUNCTION(SetNextWindowPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowSize(const Vector2 &in size, int cond "
					       "= 0)",
					       asFUNCTION(SetNextWindowSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowContentSize(const Vector2 &in size)",
					       asFUNCTION(SetNextWindowContentSize), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowCollapsed(bool collapsed, int cond = "
					       "0)",
					       asFUNCTION(SetNextWindowCollapsed), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowFocus()", asFUNCTION(SetNextWindowFocus),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowScroll(const Vector2 &in scroll)",
					       asFUNCTION(SetNextWindowScroll), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetNextWindowBgAlpha(float alpha)",
					       asFUNCTION(SetNextWindowBgAlpha), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowPos(const Vector2 &in pos, int cond = 0)",
					       asFUNCTION(SetWindowPosA), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowSize(const Vector2 &in size, int cond = 0)",
					       asFUNCTION(SetWindowSizeA), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowCollapsed(bool collapsed, int cond = 0)",
					       asFUNCTION(SetWindowCollapsedA), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowFocus()", asFUNCTION(SetWindowFocusA), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowPos(const string &in name, const Vector2 "
					       "&in pos, int cond = 0)",
					       asFUNCTION(SetWindowPosB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowSize(const string &in name, const Vector2 "
					       "&in size, int cond = 0)",
					       asFUNCTION(SetWindowSizeB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowCollapsed(const string &in name, bool "
					       "collapsed, int cond = 0)",
					       asFUNCTION(SetWindowCollapsedB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetWindowFocus(const string &in name)",
					       asFUNCTION(SetWindowFocusB), asCALL_CDECL);

		engine->RegisterGlobalFunction("float GetScrollX()", asFUNCTION(GetScrollX), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetScrollY()", asFUNCTION(GetScrollY), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollX(float scroll_x)", asFUNCTION(SetScrollX), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollY(float scroll_y)", asFUNCTION(SetScrollY), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetScrollMaxX()", asFUNCTION(GetScrollMaxX), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetScrollMaxY()", asFUNCTION(GetScrollMaxY), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollHereX(float center_x_ratio = 0.5f)",
					       asFUNCTION(SetScrollHereX), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollHereY(float center_y_ratio = 0.5f)",
					       asFUNCTION(SetScrollHereY), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollFromPosX(float local_x, float "
					       "center_x_ratio = 0.5f)",
					       asFUNCTION(SetScrollFromPosX), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetScrollFromPosY(float local_y, float "
					       "center_y_ratio = 0.5f)",
					       asFUNCTION(SetScrollFromPosY), asCALL_CDECL);

		engine->RegisterGlobalFunction("Vector2 GetFontTexUvWhitePixel()", asFUNCTION(GetFontTexUvWhitePixel),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("uint GetColorU32(int idx, float alpha_mul = 1.0f)",
					       asFUNCTION(GetColorU32A), asCALL_CDECL);
		engine->RegisterGlobalFunction("uint GetColorU32(float r, float g, float b, float a)",
					       asFUNCTION(GetColorU32B), asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetStyleColorVec4(int idx, float &out r, float &out "
					       "g, float &out b, float &out a)",
					       asFUNCTION(GetStyleColorVec4), asCALL_CDECL);

		engine->RegisterGlobalFunction("Vector2 GetCursorScreenPos()", asFUNCTION(GetCursorScreenPos),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorScreenPos(const Vector2 &in pos)",
					       asFUNCTION(SetCursorScreenPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector2 GetContentRegionAvail()", asFUNCTION(GetContentRegionAvail),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector2 GetCursorPos()", asFUNCTION(GetCursorPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetCursorPosX()", asFUNCTION(GetCursorPosX), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetCursorPosY()", asFUNCTION(GetCursorPosY), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorPos(const Vector2 &in local_pos)",
					       asFUNCTION(SetCursorPos), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorPosX(float local_x)", asFUNCTION(SetCursorPosX),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCursorPosY(float local_y)", asFUNCTION(SetCursorPosY),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector2 GetCursorStartPos()", asFUNCTION(GetCursorStartPos),
					       asCALL_CDECL);

		engine->RegisterGlobalFunction("void Separator()", asFUNCTION(Separator), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SameLine(float offset_from_start_x = 0.0f, float "
					       "spacing = -1.0f)",
					       asFUNCTION(SameLine), asCALL_CDECL);
		engine->RegisterGlobalFunction("void NewLine()", asFUNCTION(NewLine), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Spacing()", asFUNCTION(Spacing), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Dummy(const Vector2 &in size)", asFUNCTION(Dummy), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Indent(float indent_w = 0.0f)", asFUNCTION(Indent), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Unindent(float indent_w = 0.0f)", asFUNCTION(Unindent),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void BeginGroup()", asFUNCTION(BeginGroup), asCALL_CDECL);
		engine->RegisterGlobalFunction("void EndGroup()", asFUNCTION(EndGroup), asCALL_CDECL);
		engine->RegisterGlobalFunction("void AlignTextToFramePadding()", asFUNCTION(AlignTextToFramePadding),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetTextLineHeight()", asFUNCTION(GetTextLineHeight),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetTextLineHeightWithSpacing()",
					       asFUNCTION(GetTextLineHeightWithSpacing), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetFrameHeight()", asFUNCTION(GetFrameHeight), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetFrameHeightWithSpacing()",
					       asFUNCTION(GetFrameHeightWithSpacing), asCALL_CDECL);

		engine->RegisterGlobalFunction("void TextUnformatted(const string &in text)",
					       asFUNCTION(TextUnformatted), asCALL_CDECL);
		engine->RegisterGlobalFunction("void TextColored(int r, int g, int b, int a, const "
					       "string &in fmt)",
					       asFUNCTION(TextColored), asCALL_CDECL);
		engine->RegisterGlobalFunction("void TextDisabled(const string &in fmt)", asFUNCTION(TextDisabled),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void TextWrapped(const string &in fmt)", asFUNCTION(TextWrapped),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void LabelText(const string &in label, const string &in "
					       "fmt)",
					       asFUNCTION(LabelText), asCALL_CDECL);
		engine->RegisterGlobalFunction("void BulletText(const string &in fmt)", asFUNCTION(BulletText),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SeparatorText(const string &in label)", asFUNCTION(SeparatorText),
					       asCALL_CDECL);

		engine->RegisterGlobalFunction("bool Button(const string &in label, const Vector2 &in "
					       "size = Vector2(0, 0))",
					       asFUNCTION(Button), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool SmallButton(const string &in label)", asFUNCTION(SmallButton),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool InvisibleButton(const string &in str_id, const "
					       "Vector2 &in size, int flags = 0)",
					       asFUNCTION(InvisibleButton), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool ArrowButton(const string &in str_id, int dir)",
					       asFUNCTION(ArrowButton), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Checkbox(const string &in label, bool &out v)",
					       asFUNCTION(Checkbox), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool CheckboxFlags(const string &in label, int &out "
					       "flags, int flags_value)",
					       asFUNCTION(CheckboxFlagsA), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool CheckboxFlags(const string &in label, uint &out "
					       "flags, uint flags_value)",
					       asFUNCTION(CheckboxFlagsB), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool RadioButton(const string &in label, bool active)",
					       asFUNCTION(RadioButtonA), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool RadioButton(const string &in label, int &out v, int "
					       "v_button)",
					       asFUNCTION(RadioButtonB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ProgressBar(float fraction, const Vector2 &in "
					       "size_arg = Vector2(-FLT_MIN, 0))",
					       asFUNCTION(ProgressBarA), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ProgressBar(float fraction, const Vector2 &in "
					       "size_arg = Vector2(-FLT_MIN, 0), const string &in "
					       "overlay = "
					       ")",
					       asFUNCTION(ProgressBarB), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Bullet()", asFUNCTION(Bullet), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool TextLink(const string &in label)", asFUNCTION(TextLink),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("bool TextLinkOpenURL(const string &in label)",
					       asFUNCTION(TextLinkOpenURLA), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool TextLinkOpenURL(const string &in label, const "
					       "string &in url)",
					       asFUNCTION(TextLinkOpenURLB), asCALL_CDECL);

		engine->RegisterGlobalFunction("bool BeginCombo(const string &in label, const string &in "
					       "preview_value, int flags = 0)",
					       asFUNCTION(BeginCombo), asCALL_CDECL);
		engine->RegisterGlobalFunction("void EndCombo()", asFUNCTION(EndCombo), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool Combo(const string &in label, int &out "
					       "current_item, array<string>@ items, int "
					       "popup_max_height_in_items = -1)",
					       asFUNCTION(Combo), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(defNamespace);
}