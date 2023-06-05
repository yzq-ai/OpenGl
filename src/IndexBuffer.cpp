#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count),m_RendererID()
{
	ASSERT(sizeof(unsigned int )==sizeof (GLuint));

	GLCall(glGenBuffers(1, &m_RendererID)); // ���ɻ����� 
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // �󶨻����� 
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,count * sizeof(unsigned int ), data,GL_STATIC_DRAW)); // ���û��������� 
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1,&m_RendererID));//ɾ��������
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // �󶨻�����
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // ��󻺳���
}
