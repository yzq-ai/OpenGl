
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
		// 顶点位置浮点型数组 
		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f,  // 0 左下角顶点 (100,100)
			200.0f, 100.0f, 1.0f, 0.0f,  // 1 左上角顶点 (200,100)
			200.0f, 200.0f, 1.0f, 1.0f,  // 2 右上角顶点 (200,200)
			100.0f, 200.0f, 0.0f, 1.0f   // 3 右下角顶点 (100,200)
		};
		// 索引缓冲区所需索引数组 
		unsigned int indices[] = {
			0, 1, 2, // 取出positions数组的 0、1、2
			2, 3, 0
		};

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
		

		VertexArray va;//使用顶点数组

		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		//设置索引缓冲区
		IndexBuffer ib(indices, 6);

		// 左:0 、 右:1280 、 下:0 、上:720
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		// 相机位置          视图矩阵 
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));//向左移动100
		// 模型矩阵 对象位置 
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));//向右移动200，向上移动200
		// 模型视图投影矩阵
		glm::mat4 mvp = proj * view * model;  


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP",mvp);



		Texture texture("res/textures/avater.png");
		texture.Bind();//绑定纹理，默认值是0
		shader.SetUniform1i("u_Texture", 0);


		//解绑了所有的东西
		va.UnBind();
		shader.UnBind();
		vb.UnBind();
		ib.UnBind();



		Renderer renderer;

		ImGui::CreateContext();//创建ImGui上下文
		ImGui_ImplGlfw_InitForOpenGL(window, true);//链接GLFW窗口
		ImGui::StyleColorsDark();//使用ImGui的“暗色模式”


		
		const char* glsl_version = "#version 330";// 需要指定glsl版本, 也就是shader中的version
		ImGui_ImplOpenGL3_Init(glsl_version);//将ImGui与OpenGL进行连接
		glm::vec3 tranlation(200, 200, 0);

		float r = 0.0f;
		float increment = 0.005;


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			renderer.Clear();


			ImGui_ImplOpenGL3_NewFrame();//告诉ImGui开始一个新的渲染帧
			ImGui_ImplGlfw_NewFrame();//告诉ImGui发生了新的GLFW窗口事件
			ImGui::NewFrame();//ImGui准备开始一个新的界面元素布局帧

			glm::mat4 model = glm::translate(glm::mat4(1.0f), tranlation);
			glm::mat4 mvp = proj * view * model; // 模型视图投影矩阵 

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp); 
			renderer.Draw(va,ib,shader);

			va.Bind();//绑定顶点数组

			ib.Bind();

		/*
			if (r > 1.0f)
				increment = -0.005;
			if (r < 0.0f)
				increment = 0.005f;

			r += increment;
		*/
			{
			

				ImGui::Begin("ImGui");//开始一个名为"ImGui"的界面布局块
				ImGui::SliderFloat3("Tranlation", &tranlation.x, 0.0f, 1100.0f);//添加了一个名为"Translation"的滑动条控件
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				

				ImGui::End();

				
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