#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 MVP;
out vec3 colour;

void main() {
	colour = vertex_colour;
    vec4 v = vec4(vertex_position, 1.0);
	gl_Position = MVP * v;
}
