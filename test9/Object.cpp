#include "Object.hpp"

using namespace Space;
using namespace glm;


Object::Object(const bool & _is_use_texture)
	:importer()
{
	is_use_texture = _is_use_texture;
	position_modelspace = vec3(0, 0, 0);
	attitude_object = rotate(mat4(1.0), 0.0f, vec3(0, 0, 1.0));
	color_object = vec3(1.0f, 1.0f, 1.0f);
	UpdateM();
}

void Object::LoadModel(const std::string & _file_path) {
	if (is_use_texture) {
		importer.Load(_file_path);
	}
	else {
		importer.Load(_file_path, color_object);
	}

}

void Object::LoadModel(
	const std::string & _file_path
	, const glm::vec3 _color) {

	importer.Load(_file_path, _color);

}

void Object::SetObjectPositionModelSpace(const glm::vec3 & _position) {
	position_modelspace = _position;
	M_translate = translate(mat4(1.0f), _position);
	M = M_translate * M_attitude;
}

void Object::SetObjectAttitude(const glm::mat4 & _attitude) {
	attitude_object = _attitude;
	M_attitude = attitude_object;
	M = M_translate * M_attitude;
}

void Object::SetObjectColor(const glm::vec3 & _color) {
	color_object = _color;
}

void Object::Render() {
	importer.Render();
}

void Object::UpdateM() {
	M_translate = translate(mat4(1.0f), position_modelspace);
	M_attitude = attitude_object;
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
	const glm::mat4 & _projection
	, const glm::mat4 & _view
	, const GLuint & _handler_unifrom_MVP
	, const GLuint & _handler_unifrom_M
	, const GLuint & _handler_unifrom_V) {
	mat4 MVP;
	mat4 MM;

	for (int i = 0; i < objects.size(); ++i) {
		MM = M * objects[i]->GetM();
		MVP = _projection * _view * MM;

		glUniformMatrix4fv(_handler_unifrom_MVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(_handler_unifrom_M, 1, GL_FALSE, &MM[0][0]);
		glUniformMatrix4fv(_handler_unifrom_V, 1, GL_FALSE, &_view[0][0]);

		objects[i]->Render();
	}

}




