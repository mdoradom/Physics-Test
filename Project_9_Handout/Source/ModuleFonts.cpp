#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

#include<string.h>

#include "SDL2_ttf/include/SDL_ttf.h"
#pragma comment(lib, "SDL2_ttf/libx86/SDL2_ttf.lib")

ModuleFonts::ModuleFonts(bool isEnabled) : Module(isEnabled)
{

}

ModuleFonts::~ModuleFonts()
{

}

bool ModuleFonts::Init()
{
	LOG("Init ttf library");
	bool ret = true;

	// load support for the PNG image format
	//int flags = TTF_HINTING_MONO;
	int init = TTF_Init();

	//if ((init & flags) != flags)
	if (init < 0)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", TTF_GetError());
		ret = false;
	}
	else
	{
		font = TTF_OpenFont("Assets/Fonts/Roboto-Medium.ttf", 18);
		if (font == NULL)
		{
			LOG("Couldn't load %d pt font from %s: %s\n",
				18, "Assets/Fonts/Roboto-Medium.ttf", SDL_GetError());
			ret = false;
		}
		TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
		TTF_SetFontOutline(font, 0);
		TTF_SetFontKerning(font, 0);
		TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
	}

	return ret;
}

bool ModuleFonts::CleanUp()
{
	LOG("Freeing fonts and ttf library");

	TTF_CloseFont(font);
	TTF_Quit();

	return false;
}

bool ModuleFonts::drawText(const char* string, SDL_Color color, int x, int y)
{
	SDL_Surface* text_surface = NULL;
	text_surface = TTF_RenderText_Solid(font, string, color);

	texture = SDL_CreateTextureFromSurface(App->render->renderer, text_surface);
	App->render->Blit(texture, x, y);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(texture);
	return true;
}
