//------------------------------- PROJECT SETTINGS --------------------------------------------
#ifndef _common_h
#define _common_h
#define IS_WIN32 1
#pragma once
#define PROJECT_NAME "Solar System"

#define INITIAL_WIDTH 1280

#define INITIAL_HEIGHT 720

#define PI 3.14159265

#define OBJ_LOCATION "objects\\"
#define TEX_LOCATION "textures\\"


// disable warnings concerning deprecation of string operations (Visual Studio)
#define _CRT_SECURE_NO_WARNINGS

//------------------------------- DEPENDENCIES -----------------------------------------------

#include <windows.h>

#include "glew.h"

#pragma comment(lib,"glew32.lib")	
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")	

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <sstream>

//------------------------------- ROUTINES -----------------------------------------------
void PrintLog(char *message);
void ThrowException(char *msg);
void ThrowException(std::string *msg);

enum CelestrialObject {
	Sun=0,Mercury,Venus,Earth,Mars,Jupiter,Saturn,Uranus,Neptune,
	Moon,Phobos,Deimos,Europa,Ganimedes,Io,Callisto,Titan,Encelatus
};

enum Textures {
	Diffuse = 0,
	Specular = 1,
	Normal = 2,
	Extra = 3,
	ShadowCube = 4
};


#endif