#version 410

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    vec3 color;
} vertexData;

uniform int renderMode;
uniform vec3 lightDirection;
uniform vec3 eyePosition;
uniform sampler2D myTexture;

layout(location = 0) out vec4 fragColor;

void main(void)
{
    if(renderMode == 0)
    {
        // Phong shading.
        const vec3 lightColor = vec3(1.0, 1.0, 1.0);
        const vec3 matDiffuse = vec3(0.55, 0.55, 0.55);
        const vec3 matSpecular = vec3(0.7, 0.7, 0.7);
        const vec3 matAmbient = vec3(0.3, 0.3, 0.3);
        const float matShiness = 25.0;

        vec3 halfway = normalize(normalize(eyePosition - vertexData.position) + normalize(lightDirection));
        vec3 diffuse = clamp(dot(lightDirection, vertexData.normal), 0.0, 1.0) * matDiffuse;
        vec3 specular = pow(clamp(dot(halfway, vertexData.normal), 0.0, 1.0), matShiness) * matSpecular;
        vec3 intensity = matAmbient + diffuse + specular;

        fragColor = vec4(intensity * lightColor, 1.0);
    }
    else if(renderMode == 1)
    {
        // Output normal.
        fragColor = vec4(vertexData.normal * 0.5 + 0.5, 1.0);
    }
    else if(renderMode == 2)
    {
        // Output depth.
        fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    }
    else if(renderMode == 3)
    {
        // Colored.
        fragColor = vec4(vertexData.color, 1.0);
    }
    else if(renderMode == 4)
    {
        // Textured.
        fragColor = texture(myTexture, vertexData.texcoord);
    }
    else if(renderMode == 5)
    {
        // Phong shading with color.
        vec3 lightColor = vertexData.color; //const casue fault?
        const vec3 matDiffuse = vec3(0.55, 0.55, 0.55);
        const vec3 matSpecular = vec3(0.7, 0.7, 0.7);
        const vec3 matAmbient = vec3(0.3, 0.3, 0.3);
        const float matShiness = 25.0;

        vec3 halfway = normalize(normalize(eyePosition - vertexData.position) + normalize(lightDirection));
        vec3 diffuse = clamp(dot(lightDirection, vertexData.normal), 0.0, 1.0) * matDiffuse;
        vec3 specular = pow(clamp(dot(halfway, vertexData.normal), 0.0, 1.0), matShiness) * matSpecular;
        vec3 intensity = matAmbient + diffuse + specular;

        fragColor = vec4(intensity * lightColor, 1.0);
    }
}

