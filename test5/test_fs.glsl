#version 410

in vec2 uv;
out vec3 color;

uniform sampler2D my_texture_sampler;

void main() {
	color = texture(my_texture_sampler, uv).rgb;
}
