#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()	
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1,float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); // 设置对应的统一变量 
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformlf(const std::string& name, int value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath); // 这里没判断文件是否能正常打开 is_open 
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) { // 找到#shader标记 
			if (line.find("vertex") != std::string::npos) { // 顶点着色器标记 
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) { // 片段着色器标记 
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id;
	// 提升作用域 
	GLCall(id = glCreateShader(type)); // 创建对应类型的着色器 
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // 设置着色器源码 
	GLCall(glCompileShader(id)); // 编译着色器 


	//编译错误处理
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // 获取日志长度
		char* message = (char*)_malloca(length * sizeof(char));//在栈上动态分配变长数组，alloca不需要free，在作用域结束时动态释放内存
		GLCall(glGetShaderInfoLog(id, length, &length, message));// 获取日志信息
		std::cout << (type == GL_VERTEX_SHADER ? "顶点" : "片段") << " 着色器编译失败！(Fialed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!)" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}
	std::cout << (type == GL_VERTEX_SHADER ? "顶点" : "片段") << "着色器编译成功！(Succed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!)" << std::endl;
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader,const std::string& fragmentShader)
{
	unsigned int program;
	GLCall(program = glCreateProgram()); // 创建程序 
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//附加着色器
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));//链接着色器到程序上
	GLCall(glValidateProgram(program));//在程序中执行验证

	GLCall(glDeleteShader(vs));//删除顶点着色器
	GLCall(glDeleteShader(fs));//删除片段着色器

	return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end()) {
		return m_UniformlocationCache[name];
	}
	int location;
	GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) { //不终止着色器，输出错误
		std::cout<<"警告:统一变量"<<name<<"不存在! " << "( Warning: uniform '" << name << "' doesn't exist )" << std::endl;
	}

	m_UniformlocationCache[name] = location;
	return location;
}
