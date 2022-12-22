#version 330 core

in vec3 color;
in vec2 texPosition;

out vec4 fragColor;

uniform sampler2D texture;

void main() {
    // fragColor = texture(texture, texPosition); // 根据纹理坐标插值采样后得到颜色
    fragColor = texture(texture, texPosition) * vec4(color, 1.0f); // 得到的纹理颜色与顶点颜色混合，来获得更有趣的效果
}