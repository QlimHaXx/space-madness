
#version 330

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normals;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float visibility;
out vec3 normal;

const float density= 0.04;
const float gradient = 10;

void main( )
{
	vec4 worldPosition = transformationMatrix * vec4(position, 1);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
    gl_Position= projectionMatrix * positionRelativeToCam;
    normal = normals;

    float distance = length(positionRelativeToCam.xyz) / 5;

    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}

#endif


#ifdef FRAGMENT_SHADER

uniform vec4 skyColor;

in float visibility;
in vec3 normal;

out vec4 fragment_color;

void main( )
{
    fragment_color= vec4(max(normal.x, -normal.x), max(normal.y,-normal.y), max(normal.z,-normal.z), 1);
    //fragment_color = vec4(normal, 1) + color;
    fragment_color = mix(skyColor, fragment_color, visibility);
}

#endif
