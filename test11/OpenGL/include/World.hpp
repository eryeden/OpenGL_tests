

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

#include "PostProcessing.hpp"

#define USE_POSTPROCESS

#define USE_SHADOWMAPPING

/*
	a fllow of shadow mapping

	#1 render the scene from the point of view if the light, only the depth of each fragment is computed.
	Redner from the point of the light source position. Only compute the depth of each fragemt

	#2 The scene is rednered as usual, but with an extra test to see it the current fragment is in the shadow.
	Test the current frament is in the shadow.

*/




namespace Space {



	struct InfoShader {

		//�V�F�[�_�[ID
		GLuint id_shader_use_texture;
		GLuint id_shader_non_texture;
		GLuint id_shader_postprocess_passthrough;
		GLuint id_shader_postprocess_filter;

		GLuint id_shader_shadowmapping_depth;
		//GLuint id_shader_shadowmapping;

		GLuint id_shader_shadowmapping2;


		//uniformID
		GLuint id_handler_uniform_MVP_use_texture;
		GLuint id_handler_uniform_V_use_texture;
		GLuint id_handler_uniform_M_use_texture;
		GLuint id_handler_uniform_lightposition_use_texture;

		GLuint id_handler_uniform_MVP_non_texture;
		GLuint id_handler_uniform_V_non_texture;
		GLuint id_handler_uniform_M_non_texture;
		GLuint id_handler_uniform_lightposition_non_texture;

		//Shadowmapping depth
		GLuint id_handler_uniform_shadowmapping_depth_Mattrix_worldspace_to_lightspace;
		GLuint id_handler_uniform_shadowmapping_depth_Model;


		//Shadowmapping
		//GLuint id_handler_uniform_shadowmapping_projection;
		//GLuint id_handler_uniform_shadowmapping_view;
		//GLuint id_handler_uniform_shadowmapping_model;
		//GLuint id_handler_uniform_shadowmapping_light_position;
		//GLuint id_handler_uniform_shadowmapping_view_position;
		//GLuint id_handler_uniform_shadowmapping_mattrix_worldspace_to_lightspace;
		//GLuint id_handler_uniform_shadowmapping_texture_depth;
		
		//Shadowmapping2
		GLuint id_handler_uniform_MVP_shadowmapping;
		GLuint id_handler_uniform_V_shadowmapping;
		GLuint id_handler_uniform_M_shadowmapping;
		GLuint id_handler_uniform_lightposition_shadowmapping;
		GLuint id_handler_uniform_Mattrix_worldspace_to_lightspace_shadowmapping;
		GLuint id_handler_uniform_texture_depthmap_shadowmapping;

	};


	class Utility {
	public:
		static const glm::mat3 A_WorldToGLSpace;
		static const glm::mat4 Att_WorldToGLSpace;
		static const glm::vec3 ConvertWorldToGLSpace(const glm::vec3 & _in);
		static const glm::mat4 ConvertWorldToGLSpace(const glm::mat4 & _in);
		static const glm::vec3 Convert_m_To_in(const glm::vec3 & _milli);
	};


	class Object {
	public:
		Object();

		void LoadModel(const std::string & _file_path);
		void Object::LoadModel(const std::string & _file_path, const glm::vec3 _color);
		void SetObjectPositionModelSpace(const glm::vec3 & _position);
		void SetObjectAttitude(const glm::mat4 & _attitude);
		void SetObjectColor(const glm::vec3 & _color);

		glm::mat4 GetM() { return M; }

		void Render(
			const struct InfoShader & _info
			, const glm::mat4 & _projection
			, const glm::mat4 & _view
			, const glm::mat4 & _model_modelspace
			, const glm::vec3 & _light_position
			, const GLfloat & _light_power
			);


		//uniform mat4 projection;
		//uniform mat4 view;
		//uniform mat4 model;
		//uniform mat4 Matrix_Worldspace_to_Lightspace;
		//uniform sampler2D shadowMap;
		//uniform vec3 lightPos;
		//uniform vec3 viewPos;
		//ForShadowMapping
		void RenderShadowMapping(
			const struct InfoShader & _info
			, const glm::mat4 & _projection
			, const glm::mat4 & _view
			, const glm::mat4 & _model_modelspace
			, const glm::mat4 & _Mattrix_worldspace_to_lightspace_shadowmapping
			, const glm::vec3 & _light_position
			, const GLfloat & _light_power
			, const GLuint & _texture_depthmap
			);

		//DepthMap�擾�������_�����O
		void RenderDepth(
			const struct InfoShader & _info
			, const glm::mat4 & _Matrix_worldspace_to_lightspace
			, const glm::mat4 & _model_modelspace
			);

		MdlImporter importer;
	private:


		//glm::vec3 position_modelspace;
		//glm::mat4 attitude_object;
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
			const struct InfoShader & _info
			, const glm::mat4 & _projection
			, const glm::mat4 & _view
			, const glm::vec3 & _light_position
			, const GLfloat & _light_power
			);


		//ForShadowMapping
		void RenderShadowMapping(
			const struct InfoShader & _info
			, const glm::mat4 & _projection
			, const glm::mat4 & _view
			, const glm::mat4 & _Mattrix_worldspace_to_lightspace_shadowmapping
			, const glm::vec3 & _light_position
			, const GLfloat & _light_power
			, const GLuint & _texture_depthmap
			);

		//DepthMap�擾�������_�����O
		void RenderDepth(
			const struct InfoShader & _info
			, const glm::mat4 & _Matrix_worldspace_to_lightspace
			);

		void AddObject(Space::Object * _object);

		void SetModelPositionWorldSpace(const glm::vec3 & _position);
		void SetModelAttitude(const glm::mat4 & _attitude);
		std::vector<Object *> objects;
	private:

		glm::vec3 position_worldspace;
		glm::mat4 attitude_model;

		glm::mat4 M;
		glm::mat4 M_translate;
		glm::mat4 M_attitude;



		void UpdateM();

		void RenderAxis();
		void RenderAxisShort();
		void InitializeAxis();
		GLuint vertex_buffer_axis;
		GLuint vertex_buffer_axis_short;
		GLuint color_buffer_axis;
		GLuint normal_buffer_axis;
	};

	class Ground {
	public:
		Ground(GLfloat _width, GLfloat _height, GLfloat _grid_interval);
		void InitChessBoard(GLfloat _width, GLfloat _height, GLfloat _interval);
		void Render();
		void RenderGrid();
		void RenderChess();

	private:


		GLuint vertex_buffer_ground;
		GLuint color_buffer_ground;
		GLuint normal_buffer_ground;

		GLuint vertex_buffer_grid;
		GLuint color_buffer_grid;
		GLuint normal_buffer_grid;

		GLuint vertex_buffer_chess;
		GLuint color_buffer_chess;
		GLuint normal_buffer_chess;

		unsigned int number_of_indices;
		unsigned int number_of_vertices_chess;


	};

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

#ifdef USE_SHADOWMAPPING
		//ShadowMapping�p�V�F�[�_�[�ǂݍ���
		void BindShaderShadowMapping(
			const std::string & _path_to_vshader_depth
			, const std::string & _path_to_fshader_depth
			, const std::string & _path_to_vshader_shadowmapping
			, const std::string & _path_to_fshader_shadowmapping
			);

		void RenderShadowMapping();
#endif

		void SetPositionCamera(const glm::vec3 & _position_cam);
		void SetPositionLight(const glm::vec3 & _position_light);
		void SetPowerLight(const GLfloat & _power);

		void AddModel(Space::Model * _model);
		void BindGround();
		void BindSky();

		void Render();

		GLuint GetPPShader_pass() { return info.id_shader_postprocess_passthrough; }
		GLuint GetPPShader_filter() { return info.id_shader_postprocess_filter; }

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

		GLuint id_shader_shadowmapping_depth;
		GLuint id_shader_shadowmapping;

		//Shadowmapping2
		GLuint id_shader_shadowmapping2;

		//uniformID
		GLuint id_handler_uniform_MVP_use_texture;
		GLuint id_handler_uniform_V_use_texture;
		GLuint id_handler_uniform_M_use_texture;
		GLuint id_handler_uniform_lightposition_use_texture;


		GLuint id_handler_uniform_MVP_non_texture;
		GLuint id_handler_uniform_V_non_texture;
		GLuint id_handler_uniform_M_non_texture;
		GLuint id_handler_uniform_lightposition_non_texture;

		glm::mat4 Projection; //�v���W�F�N�V�����s��
		glm::mat4 View;       //���_�ϊ��s��
		glm::mat4 PV; //�v���W�F�N�V�����A���_�ϊ��̐�

		GLFWwindow *window;
		GLsizei width, height;

		std::vector<Space::Model *> models; //�`�悷�郂�f��

		struct InfoShader info;

		Ground gnd;

#ifdef USE_POSTPROCESS
		PostProcessingFXAA *pp_fxaa;
#endif

#ifdef USE_SHADOWMAPPING
		//##################### FOR SHADOW MAPPING #########################

		void InitShadowMapping();


		GLuint fbo_depthmap;
		const GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
		GLuint texture_depthmap;
		glm::mat4 Matrix_worldspace_to_lightspace;
		glm::mat4 Matrix_lightprojection;
		glm::mat4 Matrix_lightview;

		//##################### FOR SHADOW MAPPING #########################
#endif

	};





};


#endif	//WORLD_HPP