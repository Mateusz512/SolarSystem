#include "scene.h"

Scene::Scene(int new_width, int new_height)
{
	width = new_width;
	height = new_height;
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	if(commonShader) delete commonShader;
	if(depthShader) delete depthShader;
	commonShader = NULL;
	depthShader = NULL;
}
//--------------------------------------------------------------------------------------------
void Scene::PrepareObjects()
{
	meshObjects.push_back(newMeshObject("floor", glm::vec3(0, 0, 0)));
	meshObjects.push_back(newMeshObject("wall", glm::vec3(0, 0, 0)));
	meshObjects.push_back(newMeshObject("ceiling", glm::vec3(0, 0, 0)));	
	meshObjects.push_back(newMeshObject("chair", glm::vec3(4, 0.38022f, 4)));
	meshObjects.push_back(newMeshObject("chair1", glm::vec3(4, 0,-4)));
}

void Scene::Draw()
{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);
	//if (err) return; // sprawdz flage bledu (np. kompilacja shadera)

	////glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	RenderShadowMapsCube();
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);

	// ustaw macierz projekcji na perspektywiczna
	commonShader->use();
	for (int i = 0; i < pointLightsCount; i++) {
		commonShader->setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
		commonShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].ambient);
		commonShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuse);
		commonShader->setVec3("pointLights[" + std::to_string(i) + "].specular", pointLights[i].specular);
		commonShader->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
		commonShader->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
		commonShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
	}
	glm::mat4 projection = glm::perspective(cameraAngle, (float)width / (float)height, near_plane, far_plane);

	glm::mat4 view = glm::lookAt(cameraPosition,
		cameraPosition + cameraDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));
	commonShader->setMat4("projection", projection);
	commonShader->setMat4("view", view);

	// set lighting uniforms
	commonShader->setVec3("lightPos", lightPos);
	commonShader->setVec3("viewPos", cameraPosition);
	commonShader->setFloat("far_plane", far_plane);

	DrawAllObjects(commonShader, selected);

	if (selected) {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		DrawObject(commonShader, selected);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		stencilShader->use();
		stencilShader->setMat4("projection", projection);
		stencilShader->setMat4("view", view);
		float tempScale = selected->scale;
		//glm::vec3 tempPos = glm::vec3(*(selected->position));
		selected->scale = tempScale * 1.1f;
		//*(selected->position) += glm::normalize(cameraPosition - tempPos) * 0.3f;
		glDisable(GL_DEPTH_TEST);
		//TransformAndDraw(stencilShader, selected);
		selected->scale = tempScale;
		//*(selected->position) = tempPos;
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}


	skybox->Draw(projection, glm::mat4(glm::mat3(view)));
}


meshObject* Scene::newMeshObject(std::string name, glm::vec3 pos) {
	meshObject* mo = new meshObject(JoinTwoStrings(OBJ_LOCATION, name),
		JoinTwoStrings(TEX_LOCATION, name + "_diff.jpg"),
		JoinTwoStrings(TEX_LOCATION, name + "_spec.jpg"),
		JoinTwoStrings(TEX_LOCATION, name + "_normal.jpg"),
		JoinTwoStrings(TEX_LOCATION, name + "_extra.jpg"));
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
	meshObject::alreadyLoadedHelper = new AlreadyLoadedHelper();

	// inicjalizacja modu�u glew
	GLenum err = glewInit();

	// przygotuj programy shaderow
	//PreparePrograms();
	commonShader = new Shader("shaders\\defaultVS.vs", "shaders\\defaultFS.fs");
	depthShader = new Shader("shaders\\depthShader.vs", "shaders\\depthShader.fs", "shaders\\depthShader.gs");
	skyboxShader = new Shader("shaders\\skybox.vs", "shaders\\skybox.fs");
	pickingShader = new Shader("shaders\\pickingVS.vs", "shaders\\pickingFS.fs");
	stencilShader = new Shader("shaders\\stencilVS.vs", "shaders\\stencilFS.fs");

	commonShader->setInt("diffuseTexture", Diffuse);
	commonShader->setInt("isAtmo", 0);
	commonShader->setInt("shadows", 1);

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	
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
	commonShader->setInt("depthMap", ShadowCube);
	
	skybox = new Skybox(skyboxShader);

	srand(static_cast <unsigned> (time(0)));
	for (int i = 0; i < pointLightsCount; i++) {
		pointLights[i].position = glm::rotate(glm::vec3(3, 5, 0), 36.0f * i, glm::vec3(0, 1, 0));
		pointLights[i].ambient = glm::vec3(
			random(0.01f,0.05f), random(0.01f, 0.05f), random(0.01f, 0.05f)); 
		pointLights[i].diffuse = glm::vec3(
			random(0.1f, 0.8f), random(0.01f, 0.8f), random(0.01f, 0.8f)); 
		pointLights[i].specular = glm::vec3(
			random(0.5f, 1.0f), random(0.5f, 1.0f), random(0.5f, 1.0f));
		pointLights[i].constant=1.0f;
		pointLights[i].linear=0.35f;
		pointLights[i].quadratic=0.44f;
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
	float displacementSensitivity = 0.1f;
	if (key == ESCAPE) ThrowException("Zatrzymaj program");
	switch (key)
	{
		case 65: { // LEFT
			if (selected)
				Move(selected->position, glm::vec3(-1, 0, 0) * displacementSensitivity, selected->radious);
			break; 
		} 
		case 87: { // UP
			if (selected)
				Move(selected->position, glm::vec3(0, 0, -1) * displacementSensitivity, selected->radious);
			break;  } 
		case 68: { // RIGHT
			if (selected)
				Move(selected->position, glm::vec3(1, 0, 0) * displacementSensitivity, selected->radious);
			break;  } 
		case 83: { // DOWN
			if (selected)
				Move(selected->position, glm::vec3(0, 0, 1) * displacementSensitivity, selected->radious);
			break;  } 
		case 86: { break; } //T
		case 69: { if (selected) *(selected->rotationMatrix) = glm::rotate(*(selected->rotationMatrix), 10.0f, glm::vec3(0, 1.0f, 0)); break; } //E
		case 81: { if (selected) *(selected->rotationMatrix) = glm::rotate(*(selected->rotationMatrix), -10.0f, glm::vec3(0, 1.0f, 0)); break; } //Q
		case 82: { break; } //R
		case 112: { LightAmbient += 0.1f; break; } // F1		
		case 113: { LightAmbient -= 0.1f; break; } //F2		

		case 114: { break; } //F3		
		case 115: { break; } //F4		

		case 116: { break; } //F5		
		case 117: { break; } //F6		

		case 38: { Move(&cameraPosition, cameraDirection * movementSensitivity); break; } //UP
		case 40: { Move(&cameraPosition, cameraDirection * -movementSensitivity); break; } //DOWN	
		case 37: { Move(&cameraPosition, glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))) * glm::vec3(1, 0, 1) * -movementSensitivity); break; } //LEFT
		case 39: { Move(&cameraPosition, glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))) * glm::vec3(1, 0, 1) * movementSensitivity); break; } //RIGHT

		case 32: { break; }
	}

}

float Scene::CheckWhatObjectWasClicked(int x, int y) {
	unsigned char Pixel[4];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Pixel);

	if (Pixel[0] >= 3) {
		meshObject* clicked = NULL;
		for (int i = 0; i < meshObjects.size(); i++) {
			if (meshObjects[i]->ID == Pixel[0]) {
				clicked = meshObjects[i];
				break;
			}
		}
		if (!clicked) return 0;
		selected = clicked;
	}
	else {
		selected = NULL;
	}
	return	Pixel[0];
}

void Scene::MouseMoved(int x, int y, DraggingMode dr) {
	currentMousePosition.x = x; currentMousePosition.y = y;
	glm::vec2 delta = currentMousePosition - previousMousePosition;
	switch (dr) {
		case Right:
		case Left:
			cameraDirection = glm::rotate(cameraDirection, delta.x*mouseSensitivity, glm::vec3(0, 1, 0));
			cameraDirection = glm::rotate(cameraDirection, delta.y*mouseSensitivity,
				glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))));
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
	float dummy = CheckWhatObjectWasClicked(x, y);
}
//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::RenderPickingBuffer() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);
	pickingShader->use();
	
	glm::mat4 projection = glm::perspective(cameraAngle, (float)width / (float)height, near_plane, far_plane);
	glm::mat4 view = glm::lookAt(cameraPosition,
		cameraPosition + cameraDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));
	pickingShader->setMat4("projection", projection);
	pickingShader->setMat4("view", view);

	DrawAllObjects(pickingShader);
}

void Scene::Move(glm::vec3* position, glm::vec3 direction, float radious) {
	glm::vec3 temp = *position + direction * glm::vec3(1, 0, 1);
	if (abs(temp.x) + radious <= ROOM_SIZE && abs(temp.z) + radious <= ROOM_SIZE) {
		position->x = temp.x;
		position->z = temp.z;
	}
}

void Scene::DrawAllObjects(Shader* shader, meshObject* without) {

	for (int i = 0; i < meshObjects.size(); i++) {
		if (meshObjects[i] != without) {
			DrawObject(shader, meshObjects[i]);
		}
	}
}

void Scene::DrawObject(Shader* shader, meshObject* toDraw) {
	glm::mat4 mTransform=glm::mat4(1);
	mTransform = glm::translate(mTransform, (glm::vec3)(*toDraw->position));
	mTransform = mTransform * (*toDraw->rotationMatrix);
	if (toDraw->scale != 1.0f && toDraw->scale>0)
		mTransform = glm::scale(mTransform, toDraw->scale, toDraw->scale, toDraw->scale);
	shader->use();
	shader->setMat4("normalMatrix", glm::transpose(glm::inverse(mTransform)));
	shader->setMat4("model", mTransform);
	shader->setInt("EnableDiffuseTexture", 0);
	shader->setInt("EnableSpecularTexture", 0);
	shader->setInt("EnableNormalTexture", 0);
	shader->setInt("EnableExtraTexture", 0);
	shader->setInt("shadows", 1);
	for (int i = Diffuse; i <= Extra; i++) {
		GLuint textureID = toDraw->getTextureID(i);
		if (textureID == UINT_MAX){
			continue;		
		}
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureID);
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

void Scene::RenderShadowMapsCube() {
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
	DrawAllObjects(depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//------------------------------- KONIEC PLIKU -----------------------------------------------


float Scene::random(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}