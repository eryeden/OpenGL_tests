#ifndef POSTPROCESSING_HPP
#define POSTPROCESSING_HPP

//Widnows�̏ꍇ
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
//GLEW�̊g�����g���ɂ͍��ꁪ���K�v�iglGenBuffers���j
//Linux�ł͂��ꂾ���ŗǂ��@�����炭GLEW�̃��C�u�����s�K�v
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PostProcessing {
public:

	PostProcessing(const GLuint & _shader_fxaa_id, const GLuint & _shader_passthrough_id);
	PostProcessing(const std::string & _path_to_vertexshader_fxaa
		, const std::string & _path_to_fragmentshader_fxaa
		, const std::string & _path_to_vertexshader_passthrough
		, const std::string & _path_to_fragmentshader_passthrough);

	~PostProcessing();

	void Reshape(
		GLsizei width
		, GLsizei height);

	void Bind();
	void DeBind();

	void Render();

	int execute();
private:

	GLsizei width;
	GLsizei height;


	GLuint shader_fxaa;
	GLuint shader_passthrough;

	GLuint framebuffer_object_id;
	GLuint texturebuffer_id;
	GLuint render_depthbuffer_id;
	GLuint quad_vertexbuffer_id;

	//GLenum drawbuffers[1];
	//static const GLfloat g_quad_vertex_buffer_data[];
	///*= {
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	1.0f,  1.0f, 0.0f,
	//};*/

	GLuint vertexbuffer_id;

	GLuint handler_texture_passthrough;
	GLuint handler_texture_faxx;

	GLuint hundler_width_screen;
	GLuint hundler_height_screen;


	void Initialize();



};



#endif //POSTPROCESSING




































