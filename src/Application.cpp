
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"//测试

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
	std::cout<<"GLEW使用的版本是:" << "( Status: Using GLEW )" << glewGetString(GLEW_VERSION) << std::endl;
	unsigned char* glVersion;
	GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
	std::cout<<"GL使用的版本是:" << "( Status: Using GL )" << glVersion << std::endl;


	{ //限定作用域
		

		/*
		混合:将输出颜色(判断着色器输出的颜色)和目标缓冲区已有的颜色结合
		glEnable/glDisable(启用&关闭) => glBlendFunc(指定颜色因子) => glBlendEquation(指定混合模式)
		glBlendEquation(mode) mode: src和dest的混合方式(默认GL_FUNC_ADD, 叠加)
		*/
		GLCall(glEnable(GL_BLEND));//启用混合(默认不会启用)
		/*
		glBlendFunc(src, dest) 指定颜色因子
		src 指定输出颜色(RGBA)因子的计算方式, 默认为GL_ONE
		dest 指定目标颜色因子的计算方式, 默认为GL_ZERO
		GL_SRC_ALPHA 因为src的alpha为0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
		RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
		*/
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//使用混合
		


		Renderer renderer;

		ImGui::CreateContext();//创建ImGui上下文
		ImGui_ImplGlfw_InitForOpenGL(window, true);//链接GLFW窗口
		ImGui::StyleColorsDark();//使用ImGui的“暗色模式”


		
		const char* glsl_version = "#version 330";// 需要指定GLSL版本, 也就是shader中的version
		ImGui_ImplOpenGL3_Init(glsl_version);//将ImGui与OpenGL进行连接

		test::TestClearColor test;

		glm::vec3 tranlationA(200, 200, 0);
		glm::vec3 tranlationB(400, 200, 0);

		
		while (!glfwWindowShouldClose(window)) {
			renderer.Clear();

			test.OnUpdate(0.0f);
            test.OnRender();

			ImGui_ImplOpenGL3_NewFrame();//告诉ImGui开始一个新的渲染帧
			ImGui_ImplGlfw_NewFrame();//告诉ImGui发生了新的GLFW窗口事件
			ImGui::NewFrame();//ImGui准备开始一个新的界面元素布局帧
			
			{
				test.OnImGuiRender();
			}

			ImGui::Render();//将当前的UI布局帧渲染为一个完整的UI图像
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//将最近一次生成的UI图形数据绘制到OpenGL上下文中  


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}