#version 450 core

//in vec2 texCoordVertTC;
in vec2 texCoordGeomFrag;
in vec3 normal;
in vec3 pos;

out vec4 colour;

uniform sampler2D heightMap;

vec3 lightPos = vec3(0.0, 4.0, 0.0);
vec3 lightColour = vec3(0.6f, 0.6f, 0.6f);

void main() {
	//colour = vec4(texCoordGeomFrag, 0.0f, 1.0f);
	//colour = texture(heightMap, texCoordGeomFrag);
	//colour = vec4(0.0f, 0.6f, 0.1f, 1.0f);

	vec3 ambient = 0.1 * lightColour;

	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	colour = vec4((ambient + diffuse) * vec3(0.0, 0.6, 0.1), 1.0);
}