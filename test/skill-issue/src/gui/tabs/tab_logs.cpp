#include "../../imgui/texteditor/TextEditor.h"
#include "../../features/logs/logs.h"

void DrawLogsTab()
{
	static TextEditor textbox;
	static bool initialized = false;

	if (!initialized)
	{
		textbox.SetReadOnly(true);
		textbox.SetShowWhitespaces(false);
		textbox.SetColorizerEnable(false);
		initialized = true;
	}

	textbox.SetText(features::logs.GetFullText());
	textbox.Render("Logs Go Here");
}