#ifndef WORLD_HPP
#define WORLD_HPP

//Widnows�̏ꍇ
#if (_MSC_VER == 1900)	 //Visual Studio 2015
#pragma comment(lib, "glfw3-vc140.lib")
#pragma comment(lib, "assimp-vc140.lib")
#endif

#if (_MSC_VER == 1800)	 //Visual Studio 2013
#pragma comment(lib, "glfw3-vc120")  // glfw3-vc120.lib
#pragma comment(lib, "assimp-vc120.lib")  //assimp-vc120.lib
#endif

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "glew32.lib")

#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#endif

#define GL_GLEXT_PROTOTYPES
//GLEW�̊g�����g���ɂ͍��ꁪ���K�v�iglGenBuffers���j
//Linux�ł͂��ꂾ���ŗǂ��@�����炭GLEW�̃��C�u�����s�K�v
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Model Loader Library
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Importer.hpp"
#include "Object.hpp"

namespace Space {

	class World {
	public:
		World(GLFWwindow * _window);
		~World();

		void BindShader(
			const std::string & _path_to_vshader_use_texture
			, const std::string & _path_to_fshader_use_texture
			, const std::string & _path_to_vshader_non_texture
			, const std::string & _path_to_fshader_non_texture);

		void BindShaderPostProcess(
			const std::string & _path_to_vshader_pasthrough
			, const std::string & _path_to_fshader_pasthrough
			, const std::string & _path_to_vshader_filter
			, const std::string & _path_to_fshader_filter);

		void SetPositionCamera(const glm::vec3 & _position_cam);
		void SetPositionLight(const glm::vec3 & _position_light);
		void SetPowerLight(const GLfloat & _power);

		void AddModel(Space::Model * _model);
		void BindGround();
		void BindSky();

		void Render();

	private:

		//�J�����ꏊ
		glm::vec3 position_camera;
		//�����ꏊ
		glm::vec3 position_light;
		//���邳
		GLfloat power_light;

		//�V�F�[�_�[ID
		GLuint id_shader_use_texture;
		GLuint id_shader_non_texture;
		GLuint id_shader_postprocess_passthrough;
		GLuint id_shader_postprocess_filter;

		//UnifromID
		GLuint id_handler_unifrom_MVP_use_texture;
		GLuint id_handler_unifrom_V_use_texture;
		GLuint id_handler_unifrom_M_use_texture;
		GLuint id_handler_unifrom_lightposition_use_texture;

		GLuint id_handler_unifrom_MVP_non_texture;
		GLuint id_handler_unifrom_V_non_texture;
		GLuint id_handler_unifrom_M_non_texture;
		GLuint id_handler_unifrom_lightposition_non_texture;

		glm::mat4 Projection; //�v���W�F�N�V�����s��
		glm::mat4 View;       //���_�ϊ��s��
		glm::mat4 PV; //�v���W�F�N�V�����A���_�ϊ��̐�

		GLFWwindow *window;
		GLsizei width, height;

		std::vector<Space::Model *> models; //�`�悷�郂�f��

		void RenderUseTexture(); //�e�N�X�`���`��
		void RenderNontexture(); //�e�N�X�`���s�g�p

		





	};

}


#endif	//WORLD_HPP