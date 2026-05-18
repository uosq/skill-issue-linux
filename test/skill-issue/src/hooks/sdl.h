#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include "../imgui/imgui.h"

extern SDL_Window* tfwindow;

extern ImFont* IMFONT_TF2Build;
extern ImFont* IMFONT_Arial;

void SetupImGuiStyle();

void Hooked_SwapWindow(SDL_Window *window);
int Hooked_PollEvent(SDL_Event *event);
void HookSDL();