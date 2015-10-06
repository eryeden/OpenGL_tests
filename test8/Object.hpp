#ifndef OBJECT_HPP
#define OBJECT_HPP
//Widnowsの場合
#if (_MSC_VER == 1900)	 //Visual Studio 2015
#pragma comment(lib, "glfw3-vc140.lib")
#pragma comment(lib, "assimp-vc140.lib")
#endif

#if (_MSC_VER == 1800)	 //Visual Studio 2013
#pragma comment(lib, "glfw3-vc120.lib")  
#pragma comment(lib, "assimp-vc120.lib")
#endif

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "glew32.lib")

#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>

#define GL_GLEXT_PROTOTYPES
//GLEWの拡張を使うには此れ↑が必要（glGenBuffers等）
//Linuxではこれだけで良い　おそらくGLEWのライブラリ不必要
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Importer.hpp"


class Object {
public:
	Object(const GLuint &_id_shader_non_tex, const GLuint & _id_shader_use_tex);
	
	void LoadModel(const std::string & _file_path);
	void SetModelPositionWorldspace(const glm::vec3 & _position);
	void SetModelAttitude(const glm::mat4 & _attitude);
	void SetModelColor(const glm::vec3 & _color);

	void Render();

private:
	bool is_use_texture;
	MdlImporter importer;

	glm::vec3 position_worldspace;
	glm::mat4 attitude_mdoel;
	glm::vec3 color_model;

	GLuint id_shader_program;

	GLuint id_location_MVP;
	GLuint id_location_M;
	GLuint id_location_V;

	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
	glm::mat4 MVP;

	void UpdateMVP();

	void RenderUseTexture();
	void RenderNonTexture();
	

};


#endif	//OBJECT_HPP