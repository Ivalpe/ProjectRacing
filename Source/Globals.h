#pragma once

#include "raylib.h"

#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH		 1280
#define SCREEN_HEIGHT		  720
#define SCREEN_SIZE				1
#define SCALE					2
#define CARS					9
#define UP_ANGLE				0 * PI / 180.0f;
#define RIGHT_ANGLE				90 * PI / 180.0f;
#define DOWN_ANGLE				180 * PI / 180.0f;
#define LEFT_ANGLE				-90 * PI / 180.0f;
#define WIN_FULLSCREEN		false
#define WIN_RESIZABLE		false
#define WIN_BORDERLESS		false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC				true
#define FPS					   60
#define TITLE "Vroom Vroom - Subete vamos a terapia"
#define SPRITE_WIDTH		   16
#define SPRITE_HEIGHT		   32
#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !
#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)