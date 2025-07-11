#version 460 

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo{
    mat4 projection;
    mat4 view;
    vec4 ambientLightColor;
    vec3 lightPositition;
    vec4 lightColor;
} ubo;

void main() {
    outColor = vec4(ubo.lightColor.xyz, 1.0) ;
}