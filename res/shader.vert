#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 transform;
uniform mat4 eye;
uniform mat4 projection;

out vec2 texCoordVertTC;

void main() {
	texCoordVertTC = texCoord;
	gl_Position = projection * eye * transform * vec4(pos, 1.0f);
}