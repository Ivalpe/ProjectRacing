#pragma once
#include "Module.h"

#include "raylib.h"

#define MAX_TEXTURES 50


class ModuleTextures : public Module
{
public:
	//Constructor
	ModuleTextures(Application* app, bool startEnabled = true);

	//Destructor
	~ModuleTextures();

	// Called on application start.
	// Initializes the SDL_Image library
	bool Init() override;

	// Called on application exit.
	// Frees the memory from all stored textures
	// Uninitializes the SDL_Image library
	bool CleanUp() override;

	// Loads a new SDL_Texture from a file path
	// First creates an SDL_Surface, then converts it into SDL_Texture
	// Returns nullptr if the texture could not be created
	Texture2D* const Load(const char* path);

	// Removes the memory from SDL_Texture and removes it from the array
	bool Unload(Texture2D* texture);

	// Retrieve size of a texture by modifying parameters with and height
	void GetTextureSize(Texture2D* texture, uint& width, uint& height) const;

public:
	// An array of all the loaded textures
	// Allows us to keep track of all textures and free them on application exit
	Texture2D* textures[MAX_TEXTURES] = { nullptr };
};