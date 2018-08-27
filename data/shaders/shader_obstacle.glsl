
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
    //fragment_color= vec4(max(normal.y, -normal.y), max(normal.y,-normal.y), max(normal.y,-normal.y), 1) + vec4(1, 0.6, 0, 1);
    fragment_color = vec4(normal.y, normal.y, normal.x, 1) + vec4(1, 0.7, 0.5, 1);
    fragment_color = mix(skyColor, fragment_color, visibility);
}

#endif
