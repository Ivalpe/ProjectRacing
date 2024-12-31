#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include <math.h>

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    background = RAYWHITE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	
	camera.width = App->window->GetWidth() / 2;
	camera.height = App->window->GetHeight() / 2;
	camera.x = 0;
	camera.y = 0;
	

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
    ClearBackground(background);

    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    BeginDrawing();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
    EndDrawing();

	return UPDATE_CONTINUE;
}

void Module::DrawDebug() {
	DrawFPS(10, 10);
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	return true;
}

void ModuleRender::SetBackgroundColor(Color color)
{
	background = color;
}

// Draw to screen
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

    Vector2 position = { (float)x, (float)y };
	texture.width *= SCALE;
	texture.height *= SCALE;
    Rectangle rect = { 0.f, 0.f, (float)texture.width, (float)texture.height};

    if (section != NULL) rect = *section;

    position.x = (float)(x-pivot_x) + camera.x;
    position.y = (float)(y-pivot_y) + camera.y;

	rect.width;
	rect.height;

    DrawTextureRec(texture, rect, position, WHITE);

	return ret;
}

bool ModuleRender::DrawText(const char * text, int x, int y, Font font, int spacing, Color tint) const
{
    bool ret = true;

    Vector2 position = { (float)x, (float)y };

    DrawTextEx(font, text, position, (float)font.baseSize, (float)spacing, tint);

    return ret;
}

void ModuleRender::DrawRectangleDebug(float x, float y, float width, float height, Color color) {
	DrawRectangle(x, y, width, height, color);
}


