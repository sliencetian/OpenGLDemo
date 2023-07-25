#version 300 es
layout(location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = aPos;
   ourColor = aColor;
   TexCoord = aTexCoord;
}