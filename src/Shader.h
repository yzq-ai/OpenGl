#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	//mutable 可变变量意味着可以被常量函数修改
	mutable std::unordered_map<std::string, int> m_UniformlocationCache;//缓存统一变量

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1iv(const std::string& name, int count, int* value);
	void SetUniform1i(const std::string& name,int value);
	void SetUniformlf(const std::string& name,int value);
	void SetUniformMat4f(const std::string& name,const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name) const ;
};