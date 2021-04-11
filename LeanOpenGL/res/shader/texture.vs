#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 outUV;
uniform float scale = 1;
uniform mat4 trans;
void main(){
    outUV = vec2(aUv.x, aUv.y) * scale;
    gl_Position = trans * vec4(aPos,1.0);
}
