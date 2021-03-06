
#include "World.hpp"

using namespace Space;
using namespace glm;

World::World(GLFWwindow * _window) 
	:pp_fxaa()
{
	window = _window;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	position_camera = vec3(10.0f, 10.0f, 10.0f);
	position_light = vec3(0.0f, 0.0f, 100.0f);

	glfwGetWindowSize(window, &width, &height);

	View = lookAt(position_camera
		, vec3(0, 0, 0)
		, vec3(0, 0, 1));

	Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);

}



World::~World() {
	
}

void World::BindShader(
	const std::string & _path_to_vshader_use_texture
	, const std::string & _path_to_fshader_use_texture
	, const std::string & _path_to_vshader_non_texture
	, const std::string & _path_to_fshader_non_texture) {

	id_shader_use_texture = LoadShaders(
		_path_to_vshader_use_texture.c_str()
		, _path_to_fshader_use_texture.c_str());
	id_shader_non_texture = LoadShaders(
		_path_to_vshader_non_texture.c_str()
		, _path_to_fshader_non_texture.c_str());

	info.id_shader_use_texture = id_shader_use_texture;
	info.id_shader_non_texture = id_shader_non_texture;

	info.id_handler_unifrom_MVP_use_texture 
		= glGetUniformLocation(id_shader_use_texture, "MVP");
	info.id_handler_unifrom_MVP_non_texture
		= glGetUniformLocation(id_shader_non_texture, "MVP");

	info.id_handler_unifrom_M_use_texture
		= glGetUniformLocation(id_shader_use_texture, "M");
	info.id_handler_unifrom_M_non_texture
		= glGetUniformLocation(id_shader_non_texture, "M");

	info.id_handler_unifrom_V_use_texture
		= glGetUniformLocation(id_shader_use_texture, "V");
	info.id_handler_unifrom_V_non_texture
		= glGetUniformLocation(id_shader_non_texture, "V");

	info.id_handler_unifrom_lightposition_use_texture
		= glGetUniformLocation(id_shader_use_texture, "LightPosition_worldspace");
	info.id_handler_unifrom_lightposition_non_texture
		= glGetUniformLocation(id_shader_non_texture, "LightPosition_worldspace");


}


void World::BindShaderPostProcess(
	const std::string & _path_to_vshader_pasthrough
	, const std::string & _path_to_fshader_pasthrough
	, const std::string & _path_to_vshader_filter
	, const std::string & _path_to_fshader_filter) {
	id_shader_postprocess_passthrough = LoadShaders(
		_path_to_vshader_pasthrough.c_str()
		, _path_to_fshader_pasthrough.c_str());
	id_shader_postprocess_filter = LoadShaders(
		_path_to_vshader_filter.c_str()
		, _path_to_fshader_filter.c_str());

	pp_fxaa.SetShader(
		id_shader_postprocess_filter
		, id_shader_postprocess_passthrough);
	pp_fxaa.EnableFXAA();

	info.id_shader_postprocess_passthrough = id_shader_postprocess_passthrough;
	info.id_shader_postprocess_filter = id_shader_postprocess_filter;
	
}

void World::AddModel(Space::Model * _model) {
	models.push_back(_model);  //描画モデル追加
}

//地面描画機構
void World::BindGround() {

}

//スカイドーム描画機構
void World::BindSky() {
	
}

void World::SetPositionCamera(const glm::vec3 & _position_cam) {
	position_camera = _position_cam;
}

void World::SetPositionLight(const glm::vec3 & _position_light) {
	position_light = _position_light;
}

void World::SetPowerLight(const GLfloat & _power) {
	power_light = _power;
}

void World::Render() {

	glfwGetWindowSize(window, &width, &height);
	
	pp_fxaa.Reshape(width, height);

	pp_fxaa.Bind();
	
	Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < models.size(); ++i) {
		models[i]->Render(
			info
			, Projection
			, View
			, position_light
			, power_light
			);
	}

	pp_fxaa.DeBind();

	pp_fxaa.Render();
}



