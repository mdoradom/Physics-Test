#pragma once

#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include "../External_Libraries/SDL2_ttf/include/SDL_ttf.h"

class ModuleFonts : public Module
{
public:
	// Constructor
	ModuleFonts(bool isEnabled);

	// Destructor
	~ModuleFonts();

	bool Init();
	bool CleanUp();

	bool drawText(const char* string, SDL_Color color, int x, int y);

public:
	TTF_Font* font;
	SDL_Texture* texture;
};