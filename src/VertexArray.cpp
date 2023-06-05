#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1,&m_RenderID));//���ɶ�������
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RenderID));//ɾ����������
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));//����ָ������Ϊi�ĳ��涥������
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);//ƫ����
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RenderID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
