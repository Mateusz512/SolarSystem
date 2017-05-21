#include "texture.h"

//--------------------------------------------------------------------------------------------
// Domyslny konstruktor 
glTexture::glTexture(char *FileName)
{
	loadTexture(FileName);
	/*char *ImgData = NULL;
	int ImgWidth;
	int ImgHeight;
	LoadBMP(FileName,ImgWidth,ImgHeight,&ImgData);

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, ImgData);

	free(ImgData);
	*/
	/*glGenSamplers(1, &SamplerId);

	glSamplerParameteri(SamplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glSamplerParameteri(SamplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
	/*
	if (TextureId)
		sprintf(_msg,"Zaladowano %s jako ID=%d",FileName,TextureId);
	else
		sprintf(_msg,"Nie udalo sie utworzyc tekstury z %s",FileName);

	 PrintLog(_msg);*/
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glTexture::~glTexture()
{
	/*glDeleteSamplers(1, &SamplerId);
	glDeleteTextures(1, &TextureId);*/
}
//--------------------------------------------------------------------------------------------
unsigned int glTexture::loadTexture(char * path)
{
	

	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		//TODO: DIDN'T LOAD IMAGE
		stbi_image_free(data);
	}

	return textureID;
}

void glTexture::LoadBMP(char *filename,int &_width, int &_height, char **pixels)
{
	char buf[100];  // 100B temporary buffer

	// some essential header fields
	// to be compared against the assumed bmp format
	int file_actual_size;
	int file_size;  
	int bmp_compression;

	// file handle
	FILE *fil =  NULL;
	fopen_s(&fil,filename,"rb");
	
	// check if file is opened 
	if (fil == NULL) ThrowException("Brak pliku BMP");

	fseek (fil, 0, SEEK_END);
    file_actual_size=ftell (fil); // obtain real filesize 
	rewind(fil); // make sure the file pointer is set to the beginning 

	//*********** READ THE HEADER ***************
	
	// read the signature 
	fread(buf,1,2,fil); 	
	if ((buf[0] !=0x42) || (buf[1] != 0x4d)) {fclose(fil); ThrowException("Invalid signature");}

	// read the filesize
	fread(&file_size,sizeof(file_size),1,fil);
	if (file_size != file_actual_size) {ThrowException("Invalid file size");}

	// 12 dummies
	fread(buf,1,12,fil);

	fread(&_width,sizeof(_width),1,fil); 
	fread(&_height,sizeof(_height),1,fil); 
	
	// 2 dummies
	fread(buf,1,2,fil);

	// bits per pixel
	fread(buf,1,1,fil);
	if (buf[0] != 24) ThrowException("Invalid BPP"); 

	// 1 dummy
	fread(buf,1,1,fil);

	//compression mode 
	fread(&bmp_compression,sizeof(bmp_compression),1,fil); 
	if (bmp_compression != 0) ThrowException("Invalid Compression");; // invalid compression

	// 20 dummies
	fread(buf,1,20,fil);

	//********************* END OF THE HEADER ***********************

	// determine padding constant 
	int padding = 0;
	if (_width*3 % 4) padding = 4 - (_width*3) % 4;

	// allocate memory for pixeltable 
	*pixels  = (char *)calloc((_width*3+padding)*_height,sizeof(char));

	// read file content into pixeltable
	for (int h = 0; h < _height; h++)
	{
		fread(*pixels+h*(_width*3+padding),1,_width*3+padding,fil);
	}
	
	fclose(fil);
}
//--------------------------------------------------------------------------------------------
// ustawia teksture jako aktualna w OpenGL
GLuint glTexture::GetTextureID()
{
	/*glActiveTexture(GL_TEXTURE0); // poziom mipmapy 
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glBindSampler(0, SamplerId);*/
	return textureID;
}
//--------------------------------------------------------------------------------------------
// The end 


