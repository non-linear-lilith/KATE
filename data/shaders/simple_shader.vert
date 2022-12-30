#version 460

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout( push_constant ) uniform Push{
    mat2 transform;
    vec2 offset;
    vec3 color;
} push;

void main(){                                                           //pp is NOT CONMUTATIVE, ORDER MATTERS
    gl_Position = vec4(push.transform*position+push.offset,0.0,1.0);   //4d vector that maps to our output frame buffer image
}