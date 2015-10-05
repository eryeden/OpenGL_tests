#include "Object.hpp"



Object::Object(const GLuint & _id_shader_non_tex, const GLuint & _id_shader_use_tex)
	:importer(_id_shader_non_tex, _id_shader_use_tex)
{
	is_use_texture = false;

	id_shader_program = _id_shader_non_tex;

	id_location_MVP = glGetUniformLocation(id_shader_program, "MVP");
	id_location_M = glGetUniformLocation(id_shader_program, "M");
	id_location_V = glGetUniformLocation(id_shader_program, "V");

}

void Object::LoadModel(const std::string & _file_path) {
	
}

void Object::SetModelPositionWorldspace(const glm::vec3 & _position) {

}

void Object::SetModelAttitude(const glm::mat4 & _attitude) {
	
}

void Object::SetModelColor(const glm::vec3 & _color) {
	
}

void Object::Render() {
	if (is_use_texture) {
		RenderUseTexture();
	}
	else {
		RenderNonTexture();
	}
}

void Object::UpdateMVP() {
	
}

void Object::RenderUseTexture() {

}

void Object::RenderNonTexture() {

}






