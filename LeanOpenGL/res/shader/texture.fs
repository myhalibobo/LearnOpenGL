#version 330 core

out vec4 FragColor;
uniform sampler2D tex;
in vec2 outUV;

void main(){
    vec4 color = texture(tex,outUV);
    FragColor = color;
}
