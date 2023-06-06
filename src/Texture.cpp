#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //垂直翻转我们的纹理
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);//4代表RGBA格式

	GLCall(glGenTextures(1,&m_RendererID));//加载纹理
	GLCall(glBindTexture(GL_TEXTURE_2D,m_RendererID));//绑定2D纹理

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//缩小过滤器，需要渲染的比像素点小
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//放大过滤器，需要渲染的纹理更大
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//水平环绕，嵌入
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//平铺纹理，嵌入

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));//发送数据到GPU上
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1,&m_RendererID));//对象实例销毁时删除
}

void Texture::Bind(unsigned int slot /* =0 */) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//激活当前纹理插槽
	GLCall(glBindTexture(GL_TEXTURE_2D , m_RendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));//解绑
}
