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
	int execute();
private:

};



#endif //POSTPROCESSING




































