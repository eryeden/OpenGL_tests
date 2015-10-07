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

namespace Space {

	class Object {
	public:
		Object(const bool & is_use_texture);

		void LoadModel(const std::string & _file_path);
		void Object::LoadModel(const std::string & _file_path, const glm::vec3 _color);
		void SetObjectPositionModelSpace(const glm::vec3 & _position);
		void SetObjectAttitude(const glm::mat4 & _attitude);
		void SetObjectColor(const glm::vec3 & _color);

		glm::mat4 GetM() { return M; }

		void Render();

	private:
		bool is_use_texture;
		MdlImporter importer;

		glm::vec3 position_modelspace;
		glm::mat4 attitude_object;
		glm::vec3 color_object;

		glm::mat4 M;
		glm::mat4 M_translate;
		glm::mat4 M_attitude;

		void UpdateM();

	};

	class Model {
	public:

		Model();

		void Render(
			const glm::mat4 & _projection
			, const glm::mat4 & _view
			, const GLuint & _handler_unifrom_MVP
			, const GLuint & _handler_unifrom_M
			, const GLuint & _handler_unifrom_V);

		void AddObject(Space::Object * _object);

		void SetModelPositionWorldSpace(const glm::vec3 & _position);
		void SetModelAttitude(const glm::mat4 & _attitude);

	private:

		glm::vec3 position_worldspace;
		glm::mat4 attitude_model;

		glm::mat4 M;
		glm::mat4 M_translate;
		glm::mat4 M_attitude;

		std::vector<Object *> objects;

		void UpdateM();
	};

}

#endif	//OBJECT_HPP