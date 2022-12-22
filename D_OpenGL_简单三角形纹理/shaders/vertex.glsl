#version 330 core

in vec4 aPosition;
in vec2 aTexCoord;

out vec2 TexCoord;

void main(){
    gl_Position = aPosition;
    TexCoord = aTexCoord;
}