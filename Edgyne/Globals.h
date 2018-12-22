#ifndef __GLOBALS_H__
#define __GLOBALS_H__



// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include<iostream>

#define LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__)

void _log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define FPS_LOG_SIZE 100

#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//File formats
#define IMAGE(path)  path == "dds" || path == "DDS" || path == "png" || path == "PNG"
#define MODEL(path) path == "fbx" || path == "FBX"
#define SHADER(path) path == "txt" || path == "TXT"

//Stuff
#define CAMERA_OFFSET 0.4

// Configuration -----------
#define CAMERA_SPEED 10
#define CAMERA_ROTATION_SPEED 1.0f
#define CAMERA_SPEED_MULTIPLIER 2

#endif // !__GLOBALS_H__