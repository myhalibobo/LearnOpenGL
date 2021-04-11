#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 outUV;

void main(){
    outUV = vec2(aUv.x, aUv.y);
    gl_Position = vec4(aPos,1.0);
}
