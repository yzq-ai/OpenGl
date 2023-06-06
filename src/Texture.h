#pragma once

#include "Renderer.h"

class Texture {

private:
	unsigned int m_RendererID;
	std::string m_FilePath;//文件路径
	unsigned char* m_LocalBuffer;//纹理的本地储存
	int m_Width, m_Height, m_BPP;//宽度，高度，实际纹理的每像素位

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;	//默认绑定0号插槽
	void UnBind() const;

	inline int GetWidth()  const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};