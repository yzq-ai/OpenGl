#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 textCoord; //在位置1处取一个纹理坐标

out vec2 v_TextCoord;

uniform mat4 u_MVP;//模型视图投影矩阵

void main()
{
	gl_Position = u_MVP * position;
	v_TextCoord = textCoord;
};

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 v_TextCoord;

uniform vec4 u_Color;//颜色统一变量

uniform sampler2D u_Texture;

void main()
{
	vec4 textColor = texture(u_Texture, v_TextCoord);
	color = textColor;
};