//--------------------------------------------------------------------------------------------
//
// File: 	scene.hpp
// Author:	P. Katarzynski (CCE)
//
// Description: Plik naglowkowy dla klasy OpenGL
//
//--------------------------------------------------------------------------------------------

#ifndef _scene_h

#define _scene_h

#include "common.h"
#include "globject.h"
#include "printer.h"
#include "sphere.h"
#include "texture.h"
#include "blendObject.h"
#include "shader.h"
#include <string>
#include "Skybox.h"

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



//----------------------------------- DEFINES -------------------------------------------------
#define ESCAPE 27

//--------------------------------------------------------------------------------------------
// 								CLASS INTERFACE
//--------------------------------------------------------------------------------------------
class Scene
{

public:

Scene(int new_width,int new_height); //  domyslny konstruktor
~Scene(); // domyslny destruktor 

void Draw(); // rysuje zawartosc sceny

void renderShadowMaps();

void renderScene(Shader * shader);

void TransformAndDraw(Shader * shader, Drawable * toDraw);


void DrawPlanet(Shader * shader, glSphere * planet);

void Resize(int new_width, int new_height); // zmienia rozmiar sceny 

void Init(); // procedura inicjujaca przetwarzanie

void Animate(); // przeprowadza animacje sceny 

void KeyPressed(unsigned char key, int x, int y); // obsluga klawiatury

void MouseMoved(int x, int y,bool dragged);
void MouseRolled(int dir);
bool isFirstMouseMovement = true;

void SaveAsBmp(char *filename);

//GLuint LoadShader(GLenum type,const char *file_name); // laduje program shadera z zewnetrznego pliku 

private:

float LightAmbient;

float mouseSensitivity = 0.2;
float movementSensitivity = 0.3;

blendObject *test;
blendObject *plane;

glSphere* moon;
glSphere* earth;

glPrinter *Prn;


std::string imie = "Mateusz";

//void PreparePrograms(); // przygotowuje programy przetwarzania 
void PrepareObjects(); // przygotowuje obiekty do wyswietlenia 

// rozmiary sceny 
int width;
int height;

glm::mat4 mProjection;
glm::mat4 mModelView;

// zasoby programu przetwarzania
//GLuint program;
//GLuint vertex_shader;
//GLuint fragment_shader;

// komunikaty diagnostyczne 
char _msg[1024];
int err;

Shader* defaultShader;
Shader* depthShader;
Shader* skyboxShader;

Skybox* skybox;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthCubemap;
unsigned int depthMapFBO;

float near_plane = 0.1f;
float far_plane = 400.0f;

glm::vec3 lightPos = glm::vec3(0,3,0);
glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
float cameraAngle = 45.0f;
glm::vec3 cameraDirection = glm::normalize(glm::vec3(-1, 0, -1));

glm::vec2 currentMousePosition = glm::vec2(0, 0);
glm::vec2 previousMousePosition = glm::vec2(0, 0);

};

#endif
//------------------------------- KONIEC PLIKU -----------------------------------------------
