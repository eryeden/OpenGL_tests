
#include "World.hpp"

using namespace Space;
using namespace glm;

World::World(GLFWwindow * _window) {
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
	Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
	PV = Projection * View;


}

void World::RenderUseTexture() {
	glUseProgram(id_shader_use_texture);

}

void World::RenderNontexture() {
	glUseProgram(id_shader_non_texture);
	

}



