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
	delete defaultShader;
	delete depthShader;
}
//--------------------------------------------------------------------------------------------
// przygotowuje programy cienionwania 
/*void Scene::PreparePrograms()
{
	err = 0; // ustaw flage bledu
	program  = glCreateProgram();
	if (!glIsProgram(program)) {err = 1; ThrowException("Nie udalo sie utworzyc programu");}

	vertex_shader = LoadShader(GL_VERTEX_SHADER,"vs.glsl");
	glAttachShader(program,vertex_shader);

	fragment_shader = LoadShader(GL_FRAGMENT_SHADER,"fs.glsl");
	glAttachShader(program,fragment_shader);

	// linkowanie programu
	glLinkProgram(program);

	GLint link_status;
	glGetProgramiv( program, GL_LINK_STATUS, &link_status);
	if( link_status == GL_FALSE )
	{
		// pobranie i wyœwietlenie komunikatu b³êdu
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
		glGetProgramInfoLog( program, logLength, NULL, log );
		PrintLog(log);
		delete[] log;
		err = 1;
		ThrowException("Blad linkowania programu");
	}
	else
		PrintLog("Program zlinkowany");

	// walidowanie programu
	glValidateProgram(program);
	GLint validate_status;
	// sprawdzenie poprawnoœci walidacji obiektu programu
	glGetProgramiv( program, GL_VALIDATE_STATUS, &validate_status );
	if( validate_status == GL_FALSE )
	{
		// pobranie i wyœwietlenie komunikatu b³êdu
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
		glGetProgramInfoLog( program, logLength, NULL, log );
		PrintLog(log);
		delete[] log;
		err = 1;
		ThrowException("Blad walidacji programu");
	}
	else
		PrintLog("Program prawidlowy");

	glUseProgram(program);
}*/
//--------------------------------------------------------------------------------------------
void Scene::PrepareObjects()
{
	test = new blendObject("objects\\Cube.obj", "textures\\moon.bmp" , NULL, NULL, NULL);
	plane = new blendObject("objects\\Deseczka.obj", "textures\\moon.bmp", NULL, NULL, NULL);
	plane->setPos(glm::vec3(0, -2.5, 0));
	plane->scale = 20;
	moon = new glSphere(2, "textures\\cloudsmap.bmp", NULL, NULL, NULL ,glm::vec3(0));
	moon->setPos(glm::vec3(0, 0, 4));
	earth = new glSphere(2, "textures\\earthmap.jpg", "textures\\2k_earth_specular_map.png", NULL, "textures\\cloudsmap.bmp",glm::vec3(1));
	earth->setPos(glm::vec3(7, 0, 0));
	rock = new blendObject("objects\\rock.obj", "textures\\rock.png" , NULL, NULL, NULL, 3000);
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
/*// laduje program shadera z zewnetrznego pliku
GLuint Scene::LoadShader(GLenum type,const char *file_name)
{
  // zmienna plikowa
  FILE *fil = NULL;
  // sproboj otworzyc plik
  fil = fopen(file_name,"rb");
  // sprawdz, czy plik sie otworzyl
  sprintf(_msg,"Nie mozna otworzyc %s",file_name);
  if (fil == NULL)  ThrowException(_msg);

  // okresl rozmiar pliku
  fseek( fil, 0, SEEK_END );
  long int file_size = ftell(fil);
  // przewin na poczatek
  rewind(fil);
  // utworzenie bufora na kod zrodlowy programu shadera
  GLchar *srcBuf = new GLchar[(file_size+1)*sizeof(GLchar)];

  // przeczytanie kodu shadera z pliku
  fread(srcBuf,1,file_size,fil);

  // zamknij plik
  fclose(fil);

  // tekst programu MUSI miec NULL na koncu
  srcBuf[file_size] = 0x00;

  // utworzenie obiektu shadera
  GLuint shader = glCreateShader(type);

  // przypisanie zrodla do shadera
  glShaderSource(shader,1,const_cast<const GLchar**>(&srcBuf),NULL);

  // sprzatanie
  delete[] srcBuf;

  // proba skompilowania programu shadera
  glCompileShader(shader);

  // sprawdzenie czy sie udalo
  GLint compile_status;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);

  if (compile_status != GL_TRUE) // nie udalo sie
  {
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
		glGetShaderInfoLog( shader, logLength, NULL, log );
		sprintf(_msg,"Blad kompilacji pliku shadera %s",file_name);
		PrintLog(_msg);
		PrintLog(log);
		ThrowException("Blad kompilacji shadera");
		delete []log;
  }
  else
  {
	  sprintf(_msg,"Plik shadera %s skompilowany",file_name);
	  PrintLog(_msg);
  }

  return shader; // zwroc id shadera
}*/
//--------------------------------------------------------------------------------------------
// inicjuje proces renderowania OpenGL
void Scene::Init()
{
	// inicjalizacja modu³u glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		sprintf(_msg, "GLew error: %s\n", glewGetErrorString(err));
		err = 1;
		ThrowException(_msg);
	}

	// pobierz informacje o wersji openGL 
	/*sprintf(_msg,"OpenGL vendor: ");
	strcat(_msg,(const char*)glGetString( GL_VENDOR ));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL renderer: ");
	strcat(_msg,(const char*)glGetString( GL_RENDERER));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL version: ");
	strcat(_msg,(const char*)glGetString( GL_VERSION));
	PrintLog(_msg);*/

	//  ustaw kolor tla sceny (RGB Z=1.0)
	glClearColor(0, 0, 0, 0.0f);

	// przygotuj programy shaderow
	//PreparePrograms();
	defaultShader = new Shader("shaders\\defaultVS.vs", "shaders\\defaultFS.fs");
	depthShader = new Shader("shaders\\depthShader.vs", "shaders\\depthShader.fs", "shaders\\depthShader.gs");
	skyboxShader = new Shader("shaders\\skybox.vs", "shaders\\skybox.fs");

	defaultShader->setInt("diffuseTexture", Diffuse);
	defaultShader->setInt("isAtmo", 0);

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


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
	
	cameraPosition = glm::vec3(5, 0, 5);

	skybox = new Skybox(skyboxShader);


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
		SaveAsBmp("elo.bmp");
		break;
	}
	}

}

void Scene::MouseMoved(int x, int y, bool dragged) {
	currentMousePosition.x = x; currentMousePosition.y = y;
	if (dragged) {
		glm::vec2 delta = currentMousePosition - previousMousePosition;
		cameraDirection = glm::rotate(cameraDirection, delta.x*mouseSensitivity, glm::vec3(0, 1, 0));
		cameraDirection = glm::rotate(cameraDirection, delta.y*mouseSensitivity,
			glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0))));
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
//--------------------------------------------------------------------------------------------
void Scene::SaveAsBmp(char *filename)
{
	int _width = this->width;      // bitmap height 
	int _height = this->height;    // bitmap width
	while (_width * 3 % 4) _width--;  // adjust bmp width to meet 4B row padding rule
	int img_size = _width*_height * 3; // image size (each pixel is coded by 3Bytes)

	int storage_4B; // four byte storage used for saving integer bytes into bmp file 	
	FILE *fil; // prepare file handle
	fopen_s(&fil, filename, "wb"); // open the file in binary mode
	rewind(fil);
	//*********************** START OF THE HEADER **********************************
	// BM signature
	storage_4B = 0x00004d42; fwrite((char *)&storage_4B, 1, 2, fil);
	// file size
	storage_4B = 54 + img_size; fwrite((char *)&storage_4B, 1, 4, fil);
	// four empty bytes 
	storage_4B = 0;  fwrite((char *)&storage_4B, 1, 4, fil);
	// pixeltable address
	storage_4B = 0x00000036;  fwrite((char *)&storage_4B, 1, 4, fil);
	//bytes in the header
	storage_4B = 0x00000028;  fwrite((char *)&storage_4B, 1, 4, fil);
	//width
	storage_4B = _width; fwrite((char *)&storage_4B, 1, 4, fil);
	//height
	storage_4B = _height; fwrite((char *)&storage_4B, 1, 4, fil);
	//color panes
	storage_4B = 1; fwrite((char *)&storage_4B, 1, 2, fil);
	//bits per pixel
	storage_4B = 24; fwrite((char *)&storage_4B, 1, 2, fil);
	//R_RGB
	storage_4B = 0; fwrite((char *)&storage_4B, 1, 4, fil);
	//RAW
	storage_4B = img_size; fwrite((char *)&storage_4B, 1, 4, fil);
	//hor res
	storage_4B = 2835; fwrite((char *)&storage_4B, 1, 4, fil);
	//ver res
	storage_4B = 2835; fwrite((char *)&storage_4B, 1, 4, fil);
	//palette colors
	storage_4B = 0; fwrite((char *)&storage_4B, 1, 4, fil);
	//important colors
	storage_4B = 0; fwrite((char *)&storage_4B, 1, 4, fil);

	//*********************** END OF THE HEADER **********************************

	unsigned char *pixels; // room for pixeltable
	pixels = (unsigned char *)malloc(img_size * sizeof(unsigned char));
	// read pixels from colorbuffer
	glReadPixels(0, 0, _width, _height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	// store pixels in the file
	fwrite(pixels, 1, img_size, fil);
	fflush(fil); // clear file cache
	fclose(fil); // close the file
	free(pixels); // release memory


}
//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::Draw()
{
	if (err) return; // sprawdz flage bledu (np. kompilacja shadera)

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	renderShadowMaps();

	// ustaw macierz projekcji na perspektywiczna
	defaultShader->use();
	glm::mat4 projection = glm::perspective(cameraAngle, (float)width / (float)height, near_plane, far_plane);
	glm::mat4 view = glm::lookAt(cameraPosition,
		cameraPosition + cameraDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));
	defaultShader->setMat4("projection", projection);
	defaultShader->setMat4("view", view);
	// set lighting uniforms
	defaultShader->setVec3("lightPos", lightPos);
	defaultShader->setVec3("viewPos", cameraPosition);
	defaultShader->setFloat("far_plane", far_plane);
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);*/

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


	TransformAndDraw(shader, plane);

	//TransformAndDraw(shader, test);

	TransformAndDraw(shader, moon);

	TransformAndDraw(shader, rock);

	DrawPlanet(shader, earth);
}

void Scene::TransformAndDraw(Shader* shader, Drawable* toDraw) {

	shader->setInt("EnableDiffuseTexture", 0);
	shader->setInt("EnableSpecularTexture", 0);
	shader->setInt("EnableNormalTexture", 0);
	shader->setInt("EnableExtraTexture", 0);

	glm::mat4 mTransform=glm::mat4(1);
	mTransform = glm::translate(mTransform, (glm::vec3)(*toDraw->position));
	mTransform = (*toDraw->rotationMatrix) * mTransform;
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
	toDraw->Draw();
}

void Scene::DrawPlanet(Shader* shader, glSphere* planet) {
	float initScale = planet->scale;
	TransformAndDraw(shader, planet);
	if (planet->hasAtmo) {
		planet->scale = initScale + initScale*0.05;
		shader->setInt("isAtmo", 1);
		shader->setVec3("atmoColor", *(planet->atmoColor));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		TransformAndDraw(shader, planet);
		glDisable(GL_BLEND);

		shader->setInt("isAtmo", 0);
		shader->setVec3("atmoColor", glm::vec3(0));
		planet->scale = initScale;
	}
}

void Scene::DrawSaturnRing() {

}
//------------------------------- KONIEC PLIKU -----------------------------------------------