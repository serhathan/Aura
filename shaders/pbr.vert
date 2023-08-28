#version 450

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec3 WorldPos;
layout(location = 2) out vec3 Normal;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
  vec3 camPos;

} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(push.modelMatrix * vec4(aPos, 1.0));
    Normal = mat3(push.normalMatrix) * aNormal;  

    gl_Position =  ubo.projection * ubo.view * vec4(WorldPos, 1.0);
}
