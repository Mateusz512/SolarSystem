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

void RenderPickingBuffer(); // rysuje bufor z ID obiektów do wybierania obiektu

void RenderShadowMapsCube(); // rysuje kostkê cieni

void DrawAllObjects(Shader * shader, meshObject* without = NULL); // rysuje wszystkie obiekty

void DrawObject(Shader * shader, meshObject * toDraw); // rysuje jeden obiekt

float CheckWhatObjectWasClicked(int x, int y); // sprawdam czy klinkê³o na jakiœ obiekt

char * JoinTwoStrings(std::string one, std::string two);

float random(float LO, float HI);

void Resize(int new_width, int new_height); // zmienia rozmiar sceny 

void Init(); // procedura inicjujaca przetwarzanie

void Animate(); // przeprowadza animacje sceny 

void KeyPressed(unsigned char key, int x, int y); // obsluga klawiatury

void Move(glm::vec3 * position, glm::vec3 direction, float radious = 0.0f); // przesuwa obiekt i sprawdza czy nie wyszed³ z pokoju

void MouseMoved(int x, int y, DraggingMode dr); // poruszenie mysz¹
void MouseRolled(int dir); // klikniêcie mysz¹
void LMBClicked(int x, int y); // lpm
bool isFirstMouseMovement = true;

private:

std::string imie = "Bartolini aka Tunczyk";

void PrepareObjects(); // przygotowuje obiekty do wyswietlenia 

meshObject * newMeshObject(std::string name, glm::vec3 pos);

// rozmiary sceny 
int width;
int height;
glm::mat4 mProjection;
glm::mat4 mModelView;
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
unsigned int depthCubemap;
unsigned int depthMapFBO;
// komunikaty diagnostyczne 
char _msg[1024];
int err;
float LightAmbient;
glm::vec3 lightPos = glm::vec3(0,2.5,10);
glm::vec3 cameraPosition = glm::vec3(5, 1.42, 5);
float cameraAngle = 45.0f;
float mouseSensitivity = 0.17f;
float movementSensitivity = 0.28f;
glm::vec3 cameraDirection = glm::normalize(glm::vec3(-1, -0.1, -1));
glm::vec2 currentMousePosition = glm::vec2(0, 0);
glm::vec2 previousMousePosition = glm::vec2(0, 0);
static const int pointLightsCount = 10;
PointLight pointLights[pointLightsCount];
std::vector<meshObject*> meshObjects;
float ROOM_SIZE = 7.5f;
meshObject* selected = NULL;
Shader* commonShader;
Shader* depthShader;
Shader* skyboxShader;
Shader* pickingShader;
Shader* stencilShader;
Skybox* skybox;
float near_plane = 0.1f;
float far_plane = 400.0f;
};

#endif
//------------------------------- KONIEC PLIKU -----------------------------------------------
