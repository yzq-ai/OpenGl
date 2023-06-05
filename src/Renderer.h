#pragma once

#include "GL/glew.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <iostream>

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



class Renderer {
public:
    void Clear()const ;
    void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader)const;
};