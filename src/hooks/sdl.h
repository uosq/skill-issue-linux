#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include "../thirdparty/imgui/imgui.h"

extern SDL_Window* tfwindow;

extern ImFont* IMFONT_TF2Build;
extern ImFont* IMFONT_Arial;

void SetupImGuiStyle();