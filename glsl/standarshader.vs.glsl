#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 color;

uniform mat4 mvp;
uniform mat4 m;

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    vec3 color;
} vertexData;

void main(void)
{
    vec4 worldPosition = m * vec4(position, 1.0);
    vertexData.position = (worldPosition / worldPosition.w).xyz; // world space position
    vertexData.normal = normalize((transpose(inverse(m)) * vec4(normal, 0.0)).xyz); // world space normal
    vertexData.texcoord = texcoord;
    vertexData.color = color;
    gl_Position = mvp * vec4(position, 1.0);
}
