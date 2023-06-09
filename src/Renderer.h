#pragma once

#include "GL/glew.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak() //断言: 调试器中断 

// 反斜杠后面不能有空格,会被编译器识别成转义字符 
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)

//清除错误
void GLClearError();
//日志回调
bool GLLogCall(const char* function, const char* file, int line);



class Renderer {
public:
    void Clear()const ;
    void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader)const;
};