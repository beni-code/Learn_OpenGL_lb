#version 330 core

in vec4 aPosition;
in vec3 aColor;
in vec2 aTexPosition;

out vec3 color;
out vec2 texPosition;

uniform mat4 transform;

void main(){
    gl_Position = transform * aPosition;
    color = aColor;
    texPosition = aTexPosition;
}