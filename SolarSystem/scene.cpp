#include "scene.h"

char* mergeTwoStrings(std::string one, std::string two) {
	const int len = one.length() + two.length();
	char* result = new char[len];
	strcpy(result, one.c_str());
	strcat(result, two.c_str());
	return result;
}

glm::vec3 getGlobalPos(Drawable* obj) {
	glm::vec3 Pos = glm::vec3(0);
	std::vector<Drawable*> parents;
	Drawable* parent = obj->parent;
	while (parent != NULL) {
		parents.push_back(parent);
		parent = parent->parent;
	}
	for (int i = 0; i < parents.size(); i++) {
		Pos += *parents[i]->position;
	}
	Pos += *obj->position;
	return Pos;
}

float random(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}
Scene::Scene(int new_width, int new_height)
{
	width = new_width;
	height = new_height;
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	if(defaultShader) delete defaultShader;
	if(depthShader) delete depthShader;
	defaultShader = NULL;
	depthShader = NULL;
}
//--------------------------------------------------------------------------------------------
void Scene::PrepareObjects()
{
	//meshObjects.push_back(new meshObject())
	//celestrials = new glSphere*[celestrialsCount];
	//celestrials[Sun] = prepareCelestrial(0.3f, "sun", glm::vec3(0), NULL, glm::vec3(0), 1, 0.02f);
	//celestrials[Mercury] = prepareCelestrial(0.3829f, "mercury", glm::vec3(0), celestrials[Sun], glm::vec3(0.39f,0,0)*x, 58.64f, 0.24f);
	//celestrials[Venus] = prepareCelestrial(0.9499f, "venus", glm::vec3(1), celestrials[Sun], glm::vec3(0.62f, 0, 0)*x, -243.02f, 0.62f);
	//celestrials[Earth] = prepareCelestrial(1.0f, "earth", glm::vec3(1), celestrials[Sun], glm::vec3(1.2, 0, 0)*x, 1.0f, 1.0f);
	//celestrials[Moon] = prepareCelestrial(0.2f, "moon", glm::vec3(0), celestrials[Earth], glm::vec3(3, 0, 0), -1.0f, 1.0f);
	//celestrials[Mars] = prepareCelestrial(0.5320f, "mars", glm::vec3(0.7,0.2,0.2), celestrials[Sun], glm::vec3(1.9f, 0, 0)*x, 1.03f, 1.88f);
	//celestrials[Jupiter] = prepareCelestrial(10.97f, "jupiter", glm::vec3(0), celestrials[Sun], glm::vec3(5.2f, 0, 0)*x, 0.41f, 11.86f);
	//celestrials[Saturn] = prepareCelestrial(9.140f, "saturn", glm::vec3(0), celestrials[Sun], glm::vec3(9.54f, 0, 0)*x, 0.43f, 29.46f);
	//celestrials[Uranus] = prepareCelestrial(3.981f, "uranus", glm::vec3(0), celestrials[Sun], glm::vec3(19.22f, 0, 0)*x, -0.72f, 84.01f);
	//celestrials[Neptune] = prepareCelestrial(3.865f, "neptune", glm::vec3(0), celestrials[Sun], glm::vec3(30.06f, 0, 0)*x, 0.67f, 167.8f);
	////celestrials[Moon] = prepareCelestrial(0.5f, "moon", glm::vec3(0), celestrials[1], glm::vec3(2,0,0), 0.2f, 0.1f);

	//saturnRing = new meshObject("objects\\rock.obj", "textures\\rock.jpg" , NULL, NULL, NULL, 10000);
	//saturnRing->setOrbitInfo(celestrials[Saturn], glm::vec3(0, 0, 0), 0.41f, 11.86f);

	//toast = new meshObject("objects\\toast.obj", "textures\\toast.jpg", NULL, NULL, NULL);
	//toast->scale = 0.02;
	//toast->setOrbitInfo(celestrials[Earth], glm::vec3(1, 0, 2), 0.41f, 11.86f);

	//satellitesCount = 12;
	//satellites = new meshObject*[satellitesCount];
	//satellites[0] = new meshObject("objects\\SpaceShip2.obj", "textures\\SpaceShip2.jpg", NULL, NULL, NULL);
	//satellites[0]->scale = 0.1;
	//satellites[0]->setOrbitInfo(celestrials[Earth], glm::vec3(2, 0, 0), 0.41f, 11.86f);
	//satellites[1] = new meshObject("objects\\SpaceShip3.obj", "textures\\SpaceShip3.jpg", NULL, NULL, NULL);
	//satellites[1]->scale = 0.1;
	//satellites[1]->setOrbitInfo(celestrials[Earth], glm::vec3(2, 0, 1), 0.41f, 11.86f);
	//satellites[2] = new meshObject("objects\\hst.obj", "textures\\universalTexturef.jpg", NULL, NULL, NULL);
	//satellites[2]->scale = 0.1;
	//satellites[2]->setOrbitInfo(celestrials[Earth], glm::vec3(1, 0, 2), 0.41f, 11.86f);
	//satellites[3] = new meshObject("objects\\SpaceShip.obj", "textures\\universalTexturef.jpg", NULL, NULL, NULL);
	//satellites[3]->scale = 0.1;
	//satellites[3]->setOrbitInfo(celestrials[Earth], glm::vec3(0, 0, 2), 0.41f, 11.86f);
	//satellites[4] = new meshObject("objects\\SpaceShip4.obj", "textures\\SpaceShip4.jpg", NULL, NULL, NULL);
	//satellites[4]->scale = 0.05;
	//satellites[4]->setOrbitInfo(celestrials[Earth], glm::vec3(-2, 0, 0), 0.41f, 11.86f);
	//satellites[5] = new meshObject("objects\\SpaceShip5.obj", "textures\\SpaceShip5.jpg", NULL, NULL, NULL);
	//satellites[5]->scale = 0.05;
	//satellites[5]->setOrbitInfo(celestrials[Earth], glm::vec3(-2, 0, -1), 0.41f, 11.86f);
	//satellites[6] = new meshObject("objects\\SpaceShip6.obj", "textures\\universalTexturef.jpg", NULL, NULL, NULL);
	//satellites[6]->scale = 0.2;
	//satellites[6]->setOrbitInfo(celestrials[Earth], glm::vec3(-2, 0, 1), 0.41f, 11.86f);
	//satellites[7] = new meshObject("objects\\SpaceShip7.obj", "textures\\SpaceShip7.jpg", NULL, NULL, NULL);
	//satellites[7]->scale = 0.2;
	//satellites[7]->setOrbitInfo(celestrials[Earth], glm::vec3(-1, 0, 2), 0.41f, 11.86f);
	//satellites[8] = new meshObject("objects\\SpaceShip8.obj", "textures\\universalTexturef.jpg", NULL, NULL, NULL);
	//satellites[8]->scale = 0.3;
	//satellites[8]->setOrbitInfo(celestrials[Earth], glm::vec3(2, 0, -1), 0.41f, 11.86f);
	//satellites[9] = new meshObject("objects\\SpaceShip9.obj", "textures\\SpaceShip9.jpg", NULL, NULL, NULL);
	//satellites[9]->scale = 0.3;
	//satellites[9]->setOrbitInfo(celestrials[Earth], glm::vec3(0, 0, -2), 0.41f, 11.86f);
	//satellites[10] = new meshObject("objects\\SpaceShip10.obj", "textures\\SpaceShip10.jpg", NULL, NULL, NULL);
	//satellites[10]->scale = 0.3;
	//satellites[10]->setOrbitInfo(celestrials[Earth], glm::vec3(-1.2, 0, -2), 0.41f, 11.86f);
	//satellites[11] = new meshObject("objects\\SpaceShip11.obj", "textures\\SpaceShip11.jpg", NULL, NULL, NULL);
	//satellites[11]->scale = 0.3;
	//satellites[11]->setOrbitInfo(celestrials[Earth], glm::vec3(1, 0, -2), 0.41f, 11.86f);

	meshObjects.push_back(newMeshObject("moon", glm::vec3(0, 1, 0)));
	cameraParent = meshObjects[0];

}

meshObject* Scene::newMeshObject(std::string name, glm::vec3 pos) {
	meshObject* mo = new meshObject(mergeTwoStrings(OBJ_LOCATION, name + ".obj"),
		mergeTwoStrings(TEX_LOCATION, name + "_diff.jpg"),
		mergeTwoStrings(TEX_LOCATION, name + "_spec.jpg"),
		mergeTwoStrings(TEX_LOCATION, name + "_normal.jpg"),
		mergeTwoStrings(TEX_LOCATION, name + "_extra.jpg"));
	mo->position = new glm::vec3(pos);
	return mo;
}

//--------------------------------------------------------------------------------------------
// Odpowiada za skalowanie sceny przy zmianach rozmiaru okna
void Scene::Resize(int new_width, int new_height)
{
	// przypisz nowe gabaryty do pol klasy 
	width = new_width;
	// uwzgledniaj obecnosc kontrolki wizualnej 
	height = new_height;
	// rozszerz obszar renderowania do obszaru o wymiarach 'width' x 'height'
	glViewport(0, 0, width, height);

	mProjection = glm::perspective(cameraAngle, (GLfloat)width / (GLfloat)height, near_plane, far_plane);
}
//--------------------------------------------------------------------------------------------
// inicjuje proces renderowania OpenGL
void Scene::Init()
{
	Drawable::meshManager = new MeshManager();

	// inicjalizacja modu³u glew
	GLenum err = glewInit();

	// przygotuj programy shaderow
	//PreparePrograms();
	defaultShader = new Shader("shaders\\defaultVS.vs", "shaders\\defaultFS.fs");
	depthShader = new Shader("shaders\\depthShader.vs", "shaders\\depthShader.fs", "shaders\\depthShader.gs");
	skyboxShader = new Shader("shaders\\skybox.vs", "shaders\\skybox.fs");
	pickingShader = new Shader("shaders\\pickingVS.vs", "shaders\\pickingFS.fs");

	defaultShader->setInt("diffuseTexture", Diffuse);
	defaultShader->setInt("isAtmo", 0);
	defaultShader->setInt("shadows", 1);

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/*glGenFramebuffers(1, &pickingFBO);
	glGenTextures(1, &pickingDepthTexture);
	PreparePickingBuffer();*/

	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	defaultShader->setInt("depthMap", ShadowCube);
	
	skybox = new Skybox(skyboxShader);

	srand(static_cast <unsigned> (time(0)));
	for (int i = 0; i < pointLightsCount; i++) {
		pointLights[i].position = glm::rotate(glm::vec3(3, 0, 0), 36.0f * i, glm::vec3(0, 1, 0));
		pointLights[i].ambient = glm::vec3(
			random(0.01f,0.05f), random(0.01f, 0.05f), random(0.01f, 0.05f)); 
		pointLights[i].diffuse = glm::vec3(
			random(0.1f, 0.8f), random(0.01f, 0.8f), random(0.01f, 0.8f)); 
		pointLights[i].specular = glm::vec3(
			random(0.5f, 1.0f), random(0.5f, 1.0f), random(0.5f, 1.0f));
		pointLights[i].constant=1.0f;
		pointLights[i].linear=0.09f;
		pointLights[i].quadratic=0.032f;
	}
}

//--------------------------------------------------------------------------------------------
// przeprowadza animacje sceny 
void Scene::Animate()
{

}
//--------------------------------------------------------------------------------------------
// kontrola naciskania klawiszy klawiatury
void Scene::KeyPressed(unsigned char key, int x, int y)
{
	if (key == ESCAPE) ThrowException("Zatrzymaj program");
	switch (key)
	{
	case 37: { break; }
	case 38: { break; }
	case 39: { break; }
	case 40: { break; }
	case 86: { break; } //T
	case 69: { cameraPosition *= 4*cameraParent->scale/glm::length(cameraPosition); } //E
	case 82: { cameraDirection = glm::normalize(cameraPosition)*-1.0f; break; } //R
	case 112: {LightAmbient += 0.1f; break; } // F1		
	case 113: {LightAmbient -= 0.1f; break; } //F2		

	case 114: { break; } //F3		
	case 115: { break; } //F4		

	case 116: { break; } //F5		
	case 117: { break; } //F6		

	case 87: {cameraPosition += cameraDirection*movementSensitivity; break; } //W
	case 83: {cameraPosition -= cameraDirection*movementSensitivity; break; } //S		
	case 65: {cameraPosition -= glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0)))
		*movementSensitivity; break; } //A
	case 68: {cameraPosition += glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0)))
		*movementSensitivity; break; } //D

	case 32: {
		break;
	}
	}

}

void Scene::MouseMoved(int x, int y, DraggingMode dr) {
	currentMousePosition.x = x; currentMousePosition.y = y;
	glm::vec2 delta = currentMousePosition - previousMousePosition;
	switch (dr) {
		case Left:
			cameraDirection = glm::rotate(cameraDirection, delta.x*mouseSensitivity, glm::vec3(0, 1, 0));
			cameraDirection = glm::rotate(cameraDirection, delta.y*mouseSensitivity,
				glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))));
			break;
		case Middle:
			if (abs(delta.x) > abs(delta.y)) {
			cameraPosition += glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0)))
				*movementSensitivity * 0.2f * ((delta.x>0)?-1.0f:1.0f);
			}else{
			cameraPosition += glm::normalize(glm::cross(cameraDirection, glm::vec3(1, 0, 0)))
				*movementSensitivity * 0.2f * ((delta.y>0) ? -1.0f : 1.0f);
			}
			break;
		case Right:
			if (abs(delta.x) > abs(delta.y)) {
				cameraPosition = glm::rotate(cameraPosition, ((delta.x > 0) ? -1.0f : 1.0f)*3.0f , glm::vec3(0, 1, 0));
				cameraDirection = glm::rotate(cameraDirection, ((delta.x > 0) ? -1.0f : 1.0f)*3.0f, glm::vec3(0, 1, 0));
			}
			else {
				cameraPosition = glm::rotate(cameraPosition, ((delta.y < 0) ? -1.0f : 1.0f)*2.0f, glm::normalize(glm::cross(cameraPosition, glm::vec3(0, 1, 0))));
				cameraDirection = glm::rotate(cameraDirection, ((delta.y < 0) ? -1.0f : 1.0f)*-2.0f, glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))));
			}
			break;
	}
	previousMousePosition.x = x; previousMousePosition.y = y;
	isFirstMouseMovement = false;
}

void Scene::MouseRolled(int dir) {
	if (dir > 0 && cameraAngle > 5)
		cameraAngle -= 5;
	else if (dir < 0 && cameraAngle < 155)
		cameraAngle += 5;
}

void Scene::LMBClicked(int x, int y) {
	float dummy = readMouseClickObj(x, y);
}
//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::DrawPicking() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);
	pickingShader->use();

	glm::vec3 eyePos = getGlobalPos(cameraParent);
	eyePos += cameraPosition;

	glm::mat4 projection = glm::perspective(cameraAngle, (float)width / (float)height, near_plane, far_plane);
	glm::mat4 view = glm::lookAt(eyePos,
		eyePos + cameraDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));
	pickingShader->setMat4("projection", projection);
	pickingShader->setMat4("view", view);

	DrawLamp(pickingShader);
	renderScene(pickingShader);
}

void Scene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);
	//if (err) return; // sprawdz flage bledu (np. kompilacja shadera)

	////glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	renderShadowMaps();
	

	// ustaw macierz projekcji na perspektywiczna
	defaultShader->use();
	for (int i = 0; i < pointLightsCount; i++) {
		defaultShader->setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
		defaultShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].ambient);
		defaultShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuse);
		defaultShader->setVec3("pointLights[" + std::to_string(i) + "].specular", pointLights[i].specular);
		defaultShader->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
		defaultShader->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
		defaultShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
	}
	glm::mat4 projection = glm::perspective(cameraAngle, (float)width / (float)height, near_plane, far_plane);
	//
	glm::vec3 eyePos = getGlobalPos(cameraParent);
	eyePos += cameraPosition;

	glm::mat4 view = glm::lookAt(eyePos,
		eyePos + cameraDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));
	defaultShader->setMat4("projection", projection);
	defaultShader->setMat4("view", view);

	// set lighting uniforms
	defaultShader->setVec3("lightPos", lightPos);
	defaultShader->setVec3("viewPos", eyePos);
	defaultShader->setFloat("far_plane", far_plane);
	
	DrawLamp(defaultShader);
	renderScene(defaultShader);

	skybox->Draw(projection, glm::mat4(glm::mat3(view)));


	//--------------------------------------------------
	// Rysowanie w trybie ortogonalnym
	//--------------------------------------------------
	/*glUniform1i(_ShadingMode, 1);
	glm::mat4 mOrto = glm::ortho(0.0f, float(width),
		0.0f, float(height));
	mModelView = glm::mat4(1.0);
	mTransform = glm::mat4(1.0);
	// ustaw macierz projekcji na ortogonalna
	glUniformMatrix4fv(_Projection, 1, GL_FALSE,
		glm::value_ptr(mOrto));
	// ustaw przeksztalcenia macierzowe
	glUniformMatrix4fv(_ModelView, 1, GL_FALSE,
		glm::value_ptr(mModelView*mTransform));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
		// rysowanie tekstu
	for (int i = 0; i < imie.length(); i++) {
		Prn->Draw(imie[i]);
		mTransform = glm::translate(mTransform, glm::vec3(Prn->CharWidth, 0, 0));
		glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform))));
		glUniformMatrix4fv(_ModelView, 1, GL_FALSE, glm::value_ptr(mModelView*mTransform));
	}
	std::string rot_yS = std::to_string(rot_y);
	for (int i = 0; i < rot_yS.length(); i++) {
		mTransform = glm::translate(mTransform, glm::vec3(Prn->CharWidth, 0, 0));
		Prn->Draw(rot_yS[i]);
		glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform))));
		glUniformMatrix4fv(_ModelView, 1, GL_FALSE, glm::value_ptr(mModelView*mTransform));
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);*/
}

void Scene::DrawLamp(Shader* shader) {

	/*shader->use();
	defaultShader->setInt("shadows", -1);
	glm::mat4 mTransform = glm::mat4(1);
	mTransform = (*celestrials[Sun]->rotationMatrix) * mTransform;
	if (celestrials[Sun]->scale != 1.0f && celestrials[Sun]->scale>0)
		mTransform = glm::scale(mTransform, celestrials[Sun]->scale, celestrials[Sun]->scale, celestrials[Sun]->scale);
	shader->setMat4("normalMatrix", glm::transpose(glm::inverse(mTransform)));
	shader->setMat4("model", mTransform);

	glActiveTexture(GL_TEXTURE0 + Diffuse);
	glBindTexture(GL_TEXTURE_2D, celestrials[Sun]->getTextureID(Diffuse));
	glEnable(GL_TEXTURE_2D);
	shader->setInt("EnableDiffuseTexture", 1);
	shader->setInt("diffuseTexture", Diffuse);

	shader->setInt("gObjectIndex", celestrials[Sun]->ID);
	celestrials[Sun]->Draw();
	defaultShader->setInt("shadows", 1);*/

}

void Scene::renderShadowMaps() {
	// 0. create depth cubemap transformation matrices
	// -----------------------------------------------
	glm::mat4 shadowProj = glm::perspective(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	// 1. render scene to depth cubemap
	// --------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	depthShader->use();
	for (unsigned int i = 0; i < 6; ++i)
		depthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	depthShader->setFloat("far_plane", far_plane);
	depthShader->setVec3("lightPos", lightPos);
	renderScene(depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::renderScene(Shader* shader) {

	for (int i = 0; i < meshObjects.size(); i++) {
		RenderDrawable(shader, meshObjects[i]);
	}
}

void Scene::TransformAndDraw(Shader* shader, Drawable* toDraw) {

	shader->use();
	shader->setInt("EnableDiffuseTexture", 0);
	shader->setInt("EnableSpecularTexture", 0);
	shader->setInt("EnableNormalTexture", 0);
	shader->setInt("EnableExtraTexture", 0);

	glm::mat4 mTransform=glm::mat4(1);
	Drawable* parent = toDraw->parent;
	while (parent != NULL) {
		mTransform = glm::translate(mTransform, (glm::vec3)(*parent->position));
		parent = parent->parent;
	}
	mTransform = glm::translate(mTransform, (glm::vec3)(*toDraw->position));
	mTransform = mTransform * (*toDraw->rotationMatrix);
	if (toDraw->scale != 1.0f && toDraw->scale>0)
		mTransform = glm::scale(mTransform, toDraw->scale, toDraw->scale, toDraw->scale);
	shader->setMat4("normalMatrix", glm::transpose(glm::inverse(mTransform)));
	shader->setMat4("model", mTransform);
	for (int i = 0; i < 4; i++) {
		GLuint dummy = toDraw->getTextureID(i);
		if (dummy == UINT_MAX){
			continue;		
		}
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, dummy);
		glEnable(GL_TEXTURE_2D);
		switch (i) {
		case Diffuse:
			shader->setInt("EnableDiffuseTexture", 1);
			shader->setInt("diffuseTexture", i);
			break;
		case Specular:
			shader->setInt("EnableSpecularTexture", 1);
			shader->setInt("specularTexture", i);
			break;
		case Normal:
			shader->setInt("EnableNormalTexture", 1);
			shader->setInt("normalTexture", i);
			break;
		case Extra:
			shader->setInt("EnableExtraTexture", 1);
			shader->setInt("extraTexture", i);
			break;
		}
	}
	glActiveTexture(GL_TEXTURE0+ShadowCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	glEnable(GL_TEXTURE_CUBE_MAP);
	shader->setInt("depthMap", ShadowCube);
	shader->setInt("gObjectIndex", toDraw->ID);
	toDraw->Draw();

}

void Scene::RenderDrawable(Shader* shader, Drawable* toDraw) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	TransformAndDraw(shader, toDraw);
	glDisable(GL_BLEND);

}

float Scene::readMouseClickObj(int x, int y) {

	unsigned char Pixel[4];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Pixel);
	
	if (Pixel[0] != 0) {
		Drawable* clicked = NULL;
		for (int i = 0; i < meshObjects.size(); i++) {
			if (meshObjects[i]->ID == Pixel[0]) {
				clicked = meshObjects[i];
				break;
			}
		}
		if (!clicked) return 0;
		if (clicked == cameraParent) {
			cameraPosition *= 4 * cameraParent->scale / glm::length(cameraPosition);
			cameraDirection = glm::normalize(cameraPosition)*-1.0f;
			return 0;
		}
		glm::vec3 pos =	getGlobalPos(cameraParent);
		cameraParent = clicked;
		glm::vec3 npos = getGlobalPos(clicked);
		cameraPosition = pos + cameraPosition - npos;
		//cameraDirection = glm::normalize(glm::vec3(-1, -0.1f, -1));
		return Pixel[0];
	}
	return Pixel[0];
}
//------------------------------- KONIEC PLIKU -----------------------------------------------