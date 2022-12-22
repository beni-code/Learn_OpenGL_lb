#version 330 core

in vec4 aPosition;
in vec3 aColor;
in vec2 aTexPosition;

out vec3 color;
out vec2 texPosition;

void main(){
    gl_Position = aPosition;
    color = aColor;
    texPosition = aTexPosition;
}