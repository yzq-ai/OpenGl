#include "Renderer.h"



//�������
void GLClearError()
{
	// ѭ����ȡ����:���������ǰ�ĵĴ��� 
	while (glGetError() != GL_NO_ERROR);
}
//��־�ص�
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << std::hex << error << "): "
			<< function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}