#include "Renderer.h"

//清除错误
void GLClearError()
{
	// 循环获取错误:作用是清除前文的错误 
	while (glGetError() != GL_NO_ERROR);
}
//日志回调
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout<<"[OpenGL错误] " << " [OpenGL Error]   (" << std::hex << error << "): "
			<< function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));//绘制图形
}
