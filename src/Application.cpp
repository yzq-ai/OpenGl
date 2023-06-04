#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak() //断言: 调试器中断 

// 反斜杠后面不能有空格,会被编译器识别成转义字符 
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)
//清除错误
static void GLClearError()
{
	// 循环获取错误:作用是清除前文的错误 
	while (glGetError() != GL_NO_ERROR);
}
//日志回调
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" <<std::hex<< error << "): "
			<< function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
//引入着色器源码
static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath); /* 这里没判断文件是否能正常打开 is_open */
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) { /* 找到#shader标记 */
			if (line.find("vertex") != std::string::npos) { /* 顶点着色器标记 */
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) { /* 片段着色器标记 */
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}
//编译着色器
static unsigned int CompileShader(unsigned int type,const std::string& source) {
	
	unsigned int id;
	// 提升作用域 
	GLCall(id = glCreateShader(type)); // 创建对应类型的着色器 
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // 设置着色器源码 
	GLCall(glCompileShader(id)); // 编译着色器 


	//编译错误处理
	int result;
	GLCall(glGetShaderiv(id,GL_COMPILE_STATUS,&result));
	if (result == GL_FALSE)
 	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // 获取日志长度
		char* message = (char*)_malloca(length*sizeof(char));//在栈上动态分配变长数组，alloca不需要free，在作用域结束时动态释放内存
		GLCall(glGetShaderInfoLog(id,length,&length,message));// 获取日志信息
		std::cout << (type==GL_VERTEX_SHADER ?"顶点":"片段") << " 着色器编译失败！(Fialed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!)" << std::endl;
		std::cout << message << std::endl;
	
		GLCall(glDeleteShader(id));
		return 0;
	}
	std::cout << (type == GL_VERTEX_SHADER ? "顶点" : "片段") << "着色器编译成功！(Succed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!)" << std::endl;
	return id;
}
//创建着色器
static unsigned int CreateShader(const std::string& veterxShader,const std::string& fragementShader) 
{
	unsigned int program;
	GLCall(program = glCreateProgram()); // 创建程序 
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,veterxShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragementShader);

	//附加着色器
	GLCall(glAttachShader(program,vs));
	GLCall(glAttachShader(program,fs));
	GLCall(glLinkProgram(program));//链接着色器到程序上
	GLCall(glValidateProgram(program));//在程序中执行验证

	GLCall(glDeleteShader(vs));//删除顶点着色器
	GLCall(glDeleteShader(fs));//删除片段着色器

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本指定使用 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本指定使用 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心配置文件
	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Open GL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(1);//设置垂直同步，与你的分辨率同步

	std::cout << glGetString(GL_VERSION) << std::endl;//显示使用的版本
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	unsigned char* glVersion;
	GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
	std::cout << "Status: Using GL " << glVersion << std::endl;

	//OpenGL中生成的所有东西都存在一个唯一的标识 ---> 一个整数

	 // 顶点位置浮点型数组 
	float positions[] = {
		-0.5f, -0.5f, // 0
		0.5f, -0.5f,  // 1
		0.5f, 0.5f,   // 2
		-0.5f, 0.5f,  // 3
	};
	// 索引缓冲区所需索引数组 
	unsigned int indices[] = {
		0, 1, 2, // 取出positions数组的 0、1、2
		2, 3, 0
	};


	unsigned int vao; // 保存顶点数组对象ID 
	GLCall(glGenVertexArrays(1, &vao)); // 生成顶点数组 
	GLCall(glBindVertexArray(vao)); // 绑定顶点数组




	unsigned int buffer; 	
	
	GLCall(glGenBuffers(1, &buffer)); // 生成缓冲区 
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // 绑定缓冲区 
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); // 设置缓冲区数据 
	
	
	
	GLCall(glEnableVertexAttribArray(0)); // 激活顶点属性-索引0-位置 
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // 设置顶点属性-索引0 
	

	//设置索引缓冲区
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));



	 //从文件中解析着色器源码 
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader); // 使用着色器程序 

	int location = glGetUniformLocation(shader,"u_Color");//该必须必须与着色器的实际变量名完全一致,实际获取的是变量在文件中的位置
	ASSERT(location!=-1);//检查一下当前的变量是否还存在，或者已被使用
	
	//解绑了所有的东西
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	
	
	float r = 0.0f;
	float increment = 0.005;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));//绑定着色器
		GLCall( glUniform4f(location,r, 0.3f, 0.8f, 1.0f));//设置该统一变量


		GLCall(glBindVertexArray(vao));



		GLCall(glBindBuffer(GL_ARRAY_BUFFER,buffer));//绑定顶点缓冲区
		GLCall(glEnableVertexAttribArray(0)); // 设置顶点缓冲区布局 
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // 设置顶点属性-索引0 
		GLCall(GL_ELEMENT_ARRAY_BUFFER,ibo);//绑定索引缓冲区
		
		

		if (r > 1.0f)
			increment = -0.005;
		if (r < 0.0f)
			increment = 0.005f;

		r += increment;
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));//错误代码，可运行得到错误的信息

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//正确代码

		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader)); // 删除着色器程序 
	glfwTerminate();
	return 0;
}