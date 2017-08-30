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
#include "texture.h"
#include "meshObject.h"
#include "shader.h"
#include <string>
#include "Skybox.h"

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



//----------------------------------- DEFINES -------------------------------------------------
#define ESCAPE 27


struct PointLight {
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

};
//--------------------------------------------------------------------------------------------
// 								CLASS INTERFACE
//--------------------------------------------------------------------------------------------
class Scene
{

public:

Scene(int new_width,int new_height); //  domyslny konstruktor
~Scene(); // domyslny destruktor 

void Draw(); // rysuje zawartosc sceny

void DrawPicking();

void DrawLamp(Shader* shader);

void renderShadowMaps();

void renderScene(Shader * shader, meshObject* without = NULL);

void TransformAndDraw(Shader * shader, Drawable * toDraw);

void RenderDrawable(Shader* shader, Drawable* toDraw);

float readMouseClickObj(int x, int y);

void Resize(int new_width, int new_height); // zmienia rozmiar sceny 

void Init(); // procedura inicjujaca przetwarzanie

void Animate(); // przeprowadza animacje sceny 

void KeyPressed(unsigned char key, int x, int y); // obsluga klawiatury

void Move(glm::vec3 * position, glm::vec3 direction, float radious = 0.0f);

void MouseMoved(int x, int y, DraggingMode dr);
void MouseRolled(int dir);
void LMBClicked(int x, int y);
bool isFirstMouseMovement = true;

private:

float LightAmbient;

float mouseSensitivity = 0.17f;
float movementSensitivity = 0.28f;


std::string imie = "Bartek";

void PrepareObjects(); // przygotowuje obiekty do wyswietlenia 

meshObject * newMeshObject(std::string name, glm::vec3 pos);

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
Shader* pickingShader;
Shader* stencilShader;

Skybox* skybox;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
unsigned int depthCubemap;
unsigned int depthMapFBO;

float near_plane = 0.1f;
float far_plane = 400.0f;

glm::vec3 lightPos = glm::vec3(0,2.5,10);
glm::vec3 cameraPosition = glm::vec3(5, 1.42, 5);
float cameraAngle = 45.0f;
glm::vec3 cameraDirection = glm::normalize(glm::vec3(-1, -0.1, -1));

glm::vec2 currentMousePosition = glm::vec2(0, 0);
glm::vec2 previousMousePosition = glm::vec2(0, 0);

static const int pointLightsCount = 10;
PointLight pointLights[pointLightsCount];

std::vector<meshObject*> meshObjects;

float ROOM_SIZE = 7.5f;
meshObject* selected = NULL;
};

#endif
//------------------------------- KONIEC PLIKU -----------------------------------------------
