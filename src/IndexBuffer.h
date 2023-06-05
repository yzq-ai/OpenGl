#pragma once

class IndexBuffer {

private :
	unsigned int m_RendererID;
	unsigned int m_Count;//统计存在多少个索引
public:
	IndexBuffer(const unsigned int* data,unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
	inline unsigned int GetCount()const { return m_Count; }
};