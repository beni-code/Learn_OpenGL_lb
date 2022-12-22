#version 330 core

in vec2 TexCoord;

out vec4 fragColor;

/*
 * 把纹理对象传给片段着色器
 * 采样器(Sampler)类型
  * 声明一个uniform sampler2D把一个纹理添加到片段着色器中
 */
uniform sampler2D ourTexture;

void main() {
    /**
    * GLSL内建的texture函数来采样纹理的颜色
    * 第一个参数是纹理采样器
    * 第二个参数是对应的纹理坐标
    *
    * texture函数使用纹理参数（坐标）对相应的颜色值进行采样
    */
    fragColor = texture2D(ourTexture,TexCoord);
}