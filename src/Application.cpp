
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

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
	/**
	 * 交换间隔，交换缓冲区之前等待的帧数，通常称为v-sync
	 * 默认情况下，交换间隔为0
	 * 这里设置为1，即每帧更新一次
	 **/
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


	{ //限定作用域
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


		VertexArray va;//使用顶点数组

		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		//设置索引缓冲区
		IndexBuffer ib(indices, 6);


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		//解绑了所有的东西
		va.UnBind();
		shader.UnBind();
		vb.UnBind();
		ib.UnBind();




		float r = 0.0f;
		float increment = 0.005;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();//绑定顶点数组

			ib.Bind();



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
	}
	glfwTerminate();
	return 0;
}