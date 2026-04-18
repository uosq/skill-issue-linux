#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

extern SDL_Window *tfwindow;

void SetupImGuiStyle();

void Hooked_SwapWindow(SDL_Window *window);
int Hooked_PollEvent(SDL_Event *event);
void HookSDL();