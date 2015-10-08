#include "World.hpp"

using namespace Space;
using namespace glm;


Object::Object()
	:importer()
	, M(mat4(1.0f))
	, M_translate(mat4(1.0f))
	, M_attitude(mat4(1.0f))
{
	M = M_translate * M_attitude;
	color_object = vec3(1.0f, 1.0f, 1.0f);
	UpdateM();
}

void Object::LoadModel(const std::string & _file_path) {

	importer.Load(_file_path);

}

void Object::LoadModel(
	const std::string & _file_path
	, const glm::vec3 _color) {

	importer.Load(_file_path, _color);

}

void Object::SetObjectPositionModelSpace(const glm::vec3 & _position) {
	M_translate = translate(mat4(1.0f), _position);
	M = M_translate * M_attitude;
}

void Object::SetObjectAttitude(const glm::mat4 & _attitude) {
	M_attitude = _attitude;
	M = M_translate * M_attitude;
}

void Object::SetObjectColor(const glm::vec3 & _color) {
	color_object = _color;
}

void Object::Render(
	const Space::InfoShader & _info
	, const glm::mat4 & _projection
	, const glm::mat4 & _view
	, const glm::mat4 & _model_modelspace
	, const glm::vec3 & _light_position
	, const GLfloat & _light_power
	) {
	mat4 MM = _model_modelspace * M;
	mat4 MVP = _projection * _view * MM;
	if (importer.IsUseTexture()) {
		glUseProgram(_info.id_shader_use_texture);

		glUniformMatrix4fv(_info.id_handler_unifrom_MVP_use_texture, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(_info.id_handler_unifrom_M_use_texture, 1, GL_FALSE, &MM[0][0]);
		glUniformMatrix4fv(_info.id_handler_unifrom_V_use_texture, 1, GL_FALSE, &_view[0][0]);
		glUniform3f(_info.id_handler_unifrom_lightposition_use_texture
			, _light_position.x, _light_position.y, _light_position.z);
	}
	else {
		glUseProgram(_info.id_shader_non_texture);

		glUniformMatrix4fv(_info.id_handler_unifrom_MVP_non_texture, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(_info.id_handler_unifrom_M_non_texture, 1, GL_FALSE, &MM[0][0]);
		glUniformMatrix4fv(_info.id_handler_unifrom_V_non_texture, 1, GL_FALSE, &_view[0][0]);
		glUniform3f(_info.id_handler_unifrom_lightposition_non_texture
			, _light_position.x, _light_position.y, _light_position.z);
	}

	importer.Render();
}

void Object::UpdateM() {
	M = M_translate * M_attitude;
}


Model::Model() {
	position_worldspace = vec3(0.0, 0.0, 0.0);
	attitude_model = rotate(mat4(1.0), 0.0f, vec3(0, 0, 1.0));
	UpdateM();
}

void Model::UpdateM() {
	M_translate = translate(mat4(1.0f), position_worldspace);
	M_attitude = attitude_model;
	M = M_translate * M_attitude;
}

void Model::AddObject(Space::Object * _object) {
	objects.push_back(_object);
}

void Model::SetModelPositionWorldSpace(const glm::vec3 & _position) {
	position_worldspace = _position;
	M_translate = translate(mat4(1.0f), position_worldspace);
	M = M_translate * M_attitude;
}

void Model::SetModelAttitude(const glm::mat4 & _attitude) {
	attitude_model = _attitude;
	M_attitude = attitude_model;
	M = M_translate * M_attitude;
}

void Model::Render(
	const Space::InfoShader & _info
	, const glm::mat4 & _projection
	, const glm::mat4 & _view
	, const glm::vec3 & _light_position
	, const GLfloat & _light_power
	) {

	for (size_t i = 0; i < objects.size(); ++i) {
		objects[i]->Render(_info
			, _projection
			, _view
			, M
			, _light_position
			, _light_power);
	}

}




