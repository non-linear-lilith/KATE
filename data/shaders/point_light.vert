#version 460

const vec2 OFFSETS[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

layout(location = 0) out vec2 fragOffset;

layout(set = 0, binding = 0) uniform GlobalUbo{
    mat4 projection;
    mat4 view;
    vec4 ambientLightColor;
    vec3 lightPositition;
    vec4 lightColor;
} ubo;

const float LIGHT_RADIUS = 0.1;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {
        ubo.view[0][0],
        ubo.view[1][0],
        ubo.view[2][0]
    };
    vec3 cameraUpWorld = {
        ubo.view[0][1],
        ubo.view[1][1],
        ubo.view[2][1]
    };

    vec3 positionWorld = ubo.lightPositition.xyz
    + LIGHT_RADIUS * (
        fragOffset.x * cameraRightWorld
        + fragOffset.y * cameraUpWorld
    );
    gl_Position = ubo.projection * (ubo.view * vec4(positionWorld, 1.0));
}