
#include "World.hpp"
#include "controls.hpp"

using namespace Space;
using namespace glm;


World::World(GLFWwindow * _window)
//:pp_fxaa()
	:gnd(100, 100, 1)
{
	window = _window;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);


	position_camera = vec3(10.0f, 10.0f, 10.0f);
	position_light = vec3(0.0f, 0.0f, 5.0f);

	glfwGetWindowSize(window, &width, &height);

	View = lookAt(position_camera
		, vec3(0, 0, 0)
		, vec3(0, 0, 1));

	Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);

	gnd.InitChessBoard(100, 100, 1);


}

World::~World() {
	//delete pp_fxaa;
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

#ifdef USE_POSTPROCESS
	pp_fxaa = new PostProcessingFXAA(
		id_shader_postprocess_filter
		, id_shader_postprocess_passthrough);
	pp_fxaa->EnableFXAA();
#endif

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

	computeMatricesFromInputs(window);

	//SetPositionLight(getPosition());

	//SetPositionLight(vec3(0, 50, 0));



#ifdef USE_POSTPROCESS
	pp_fxaa->Reshape(window);
	pp_fxaa->Bind();
#endif

	Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
	//View = lookAt(position_camera
	//	, vec3(0, 0, 0)
	//	, vec3(0, 0, 1));
	View = getViewMatrix();
	glViewport(0, 0, width, height);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < models.size(); ++i) {
		models[i]->Render(
			info
			, Projection
			, View
			, position_light
			, power_light
			);

		mat4 M = mat4(
			1.0f, 0.0f, 0.0f, 0.0f
			, 0.0f, 1.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.0f, 0.0f, 0.0f, 1.0f
			);

		mat4 MVP = Projection * View * M;

		glUseProgram(info.id_shader_non_texture);

		glUniformMatrix4fv(info.id_handler_unifrom_MVP_non_texture
			, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(info.id_handler_unifrom_M_non_texture
			, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(info.id_handler_unifrom_V_non_texture
			, 1, GL_FALSE, &View[0][0]);
		glUniform3f(info.id_handler_unifrom_lightposition_non_texture
			, position_light.x, position_light.y, position_light.z);

		//gnd.Render();
		//gnd.RenderGrid();
		gnd.RenderChess();
	}

#ifdef USE_POSTPROCESS
	pp_fxaa->DeBind();
	pp_fxaa->Render();
#endif


}



