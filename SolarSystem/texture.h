#ifndef texture_H
#define texture_H

#include "common.h"
#include "stb_image.h"

// klasa implementujaca obsluge obiektu tekstury w OpenGL
class glTexture
{

public:

glTexture(char *FileName); // domyslny konstruktor 
~glTexture(); // domyslny destruktor
GLuint GetTextureID(); // ustawia aktualny obiekt tekstury 


private: 
void LoadBMP(char *filename,int &_width, int &_height, char **pixels);
GLuint glTexture::loadTexture(char * path);
GLuint textureID;

char _msg[1024];
};


#endif