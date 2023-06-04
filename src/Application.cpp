#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
		char* message = (char*)alloca(length*sizeof(char));//在栈上动态分配变长数组，alloca不需要free，在作用域结束时动态释放内存
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

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	//First you need to create a valid OpenGL rendering context and call glewInit() to initialize the extension entry points. 

	std::cout << glGetString(GL_VERSION) << std::endl;

	//OpenGL中生成的所有东西都存在一个唯一的标识 ---> 一个整数

	float positions[6] = { 
		-0.5f, -0.5f, 		 
		0.0f,  0.5f, 		 
		0.5f, -0.5f };  	
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
		
	//顶点着色器，需要传递给gpu
	std::string vertexShader =
		"#version 330 core\n"//指定版本
		"\n"
		"layout(location = 0)in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		" gl_Position = position;\n"
		"}\n";
	//片段着色器
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0)out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		" color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader); /* 使用着色器程序 */


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//"GL_TRIANGLES" 画一个三角形 ，
		glDrawArrays(GL_TRIANGLES, 0, 3);//从buffer里第零组开始draw，一共3组点



		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}