#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //��ֱ��ת���ǵ�����
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);//4����RGBA��ʽ

	GLCall(glGenTextures(1,&m_RendererID));//��������
	GLCall(glBindTexture(GL_TEXTURE_2D,m_RendererID));//��2D����

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//��С����������Ҫ��Ⱦ�ı����ص�С
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//�Ŵ����������Ҫ��Ⱦ���������
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//ˮƽ���ƣ�Ƕ��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//ƽ������Ƕ��

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));//�������ݵ�GPU��
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1,&m_RendererID));//����ʵ������ʱɾ��
}

void Texture::Bind(unsigned int slot /* =0 */) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//���ǰ������
	GLCall(glBindTexture(GL_TEXTURE_2D , m_RendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));//���
}
