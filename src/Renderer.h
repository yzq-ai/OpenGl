#pragma once

#include <iostream>
#include "GL/glew.h"

#define ASSERT(x) if (!(x)) __debugbreak() //����: �������ж� 

// ��б�ܺ��治���пո�,�ᱻ������ʶ���ת���ַ� 
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)

//�������
void GLClearError();
//��־�ص�
bool GLLogCall(const char* function, const char* file, int line);
