#include "ModuleTextures.h"
#include "Application.h"
#include "ModuleRender.h"
#include "raylib.h"


ModuleTextures::ModuleTextures(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	for (uint i = 0; i < MAX_TEXTURES; ++i)
		textures[i] = nullptr;
}

ModuleTextures::~ModuleTextures()
{}

bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;


	// Check if all flags were initialized correctly
	//if ((init & flags) != flags)
	//{
	//	LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
	//	ret = false;
	//}

	return ret;
}

bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	//Free all textures sill existing in the textures array
	for (uint i = 0; i < MAX_TEXTURES; ++i)
	{
		if (textures[i] != nullptr)
		{
			UnloadTexture(*textures[i]);
			textures[i] = nullptr;
		}
	}


	return true;
}

Texture2D* const ModuleTextures::Load(const char* path)
{
	Texture2D* texture = nullptr;
	Image image = LoadImage(path);

	if (image.width == 0 || image.height == 0)
	{
		LOG("Could not load surface with path: %s", path);
	}
	else
	{
		texture = new Texture2D(LoadTextureFromImage(image));

		if (texture == NULL)
		{
			LOG("Unable to create texture from image\n");
			delete texture;
			texture = nullptr;
		}
		else
		{
			for (uint i = 0; i < MAX_TEXTURES; ++i)
			{
				if (textures[i] == nullptr)
				{
					textures[i] = texture;
					break;
				}
			}
		}

		
	}

	return texture;
}

bool ModuleTextures::Unload(Texture2D* texture)
{
	bool ret = false;

	if (texture != nullptr)
	{
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (textures[i] == texture)
			{
				textures[i] = nullptr;
				ret = true;
				break;
			}
		}
		UnloadTexture(*texture);
	}

	return ret;
}

void ModuleTextures::GetTextureSize(Texture2D* texture, uint& width, uint& height) const
{
	if (texture != nullptr) {
		width = texture->width;
		height = texture->height;
	}
	else {
		width = height = 0;
	}
}