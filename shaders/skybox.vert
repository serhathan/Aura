#version 450
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec3 inPos;


layout (location = 0) out vec3 outUVW;


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
} ubo;



layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
	 outUVW = inPos;
	// Convert cubemap coordinates into Vulkan coordinate space
	outUVW.xy *= -1.0;
	gl_Position = ubo.projection * push.modelMatrix * vec4(inPos.xyz, 1.0);
}