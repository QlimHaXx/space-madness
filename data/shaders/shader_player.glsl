
#version 330

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location= 2) in vec3 normals;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float visibility;
out vec3 normal;
out vec2 vertex_texcoord;

const float density= 0.04;
const float gradient = 10;

void main( )
{
	vec4 worldPosition = transformationMatrix * vec4(position, 1);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
    gl_Position= projectionMatrix * positionRelativeToCam;
    normal = normals;
    vertex_texcoord= texcoord;

    float distance = length(positionRelativeToCam.xyz) / 5;

    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}

#endif


#ifdef FRAGMENT_SHADER

uniform vec4 skyColor;
uniform sampler2D texture0;

in float visibility;
in vec3 normal;
in vec2 vertex_texcoord;

out vec4 fragment_color;

void main( )
{
    fragment_color = texture(texture0, vertex_texcoord);
    //fragment_color = vec4(normal, 1) * color;
    fragment_color= vec4(max(normal.y, -normal.y), max(normal.y,-normal.y), max(normal.y,-normal.y), 1) *fragment_color;
    fragment_color = mix(skyColor, fragment_color, visibility);
}

#endif
