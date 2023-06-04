#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


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
	
	unsigned int id = glCreateShader(type); /* 创建对应类型的着色器 */
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); /* 设置着色器源码 */
	glCompileShader(id); /* 编译着色器 */


	//错误处理
	int result;
	glGetShaderiv(id,GL_COMPILE_STATUS,&result);
	if (result == GL_FALSE)
 	{
		int length;
		glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
		char* message = (char*)_malloca(length*sizeof(char));//在栈上动态分配变长数组，alloca不需要free，在作用域结束时动态释放内存
		glGetShaderInfoLog(id,length,&length,message);
		std::cout << (type==GL_VERTEX_SHADER ?"顶点":"片段") << " 着色器编译失败！(Fialed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!)" << std::endl;
		std::cout << message << std::endl;
	
		glDeleteShader(id);
		return 0;
	}
	std::cout << (type == GL_VERTEX_SHADER ? "顶点" : "片段") << "着色器编译成功！(Suffculty to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!)" << std::endl;
	return id;
}
//创建着色器
static unsigned int CreateShader(const std::string& veterxShader,const std::string& fragementShader) 
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,veterxShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragementShader);

	//附加着色器
	glAttachShader(program,vs);
	glAttachShader(program,fs);
	//链接着色器到程序上
	glLinkProgram(program);
	//在程序中执行验证
	glValidateProgram(program);

	glDeleteShader(vs);//删除顶点着色器
	glDeleteShader(fs);//删除片段着色器

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Open GL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: Using GL " << glGetString(GL_VERSION) << std::endl;

	//OpenGL中生成的所有东西都存在一个唯一的标识 ---> 一个整数

	 /* 顶点位置浮点型数组 */
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};
	unsigned int buffer; 	
	glGenBuffers(1, &buffer);//生成一个缓冲区,&buffer表示buffer的id值
							 
	//就像 PS 一样在指定层上画画 	
	glBindBuffer(GL_ARRAY_BUFFER, buffer);  	//绑定buffer，表示可以使用buffer
	//specify the buffer 	
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);//GL_STATIC_DRAW表示使用静态绘制，一次修改多次使用

	
	//必须先调用此函数才可使用顶点属性，因为OpenGL是状态机得告诉它该函数的状态
	glEnableVertexAttribArray(0);

	//顶点属性
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//2: component count
															//stride 实际上是一个偏移量，类似结构体的多个实例中的一个实例的内存空间（比如说一个2维点是两个顶点组成，所以在这里是两个 float 类型长度）
		
	 //从文件中解析着色器源码 
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader); // 使用着色器程序 

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader); /* 删除着色器程序 */
	glfwTerminate();
	return 0;
}