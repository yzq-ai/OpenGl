#pragma once

#include "Renderer.h"

class Texture {

private:
	unsigned int m_RendererID;
	std::string m_FilePath;//�ļ�·��
	unsigned char* m_LocalBuffer;//����ı��ش���
	int m_Width, m_Height, m_BPP;//��ȣ��߶ȣ�ʵ�������ÿ����λ

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;	//Ĭ�ϰ�0�Ų��
	void UnBind() const;

	inline int GetWidth()  const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};