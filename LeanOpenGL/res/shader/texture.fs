#version 330 core

out vec4 FragColor;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float time;
in vec2 outUV;


void main(){
//    vec4 color = mix(texture(tex1,outUV),texture(tex2,outUV),0.5);
    FragColor = mix(texture(tex1, outUV), texture(tex2, vec2(1.0 - outUV.x, outUV.y)), 0.2);
//    FragColor = texture(tex1,outUV);
}
