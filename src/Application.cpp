
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾ָ��ʹ�� 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾ָ��ʹ�� 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�ú��������ļ�

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
	 * �������������������֮ǰ�ȴ���֡����ͨ����Ϊv-sync
	 * Ĭ������£��������Ϊ0
	 * ��������Ϊ1����ÿ֡����һ��
	 **/
	glfwSwapInterval(1);//���ô�ֱͬ��������ķֱ���ͬ��

	std::cout << glGetString(GL_VERSION) << std::endl;//��ʾʹ�õİ汾
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	std::cout<<"GLEWʹ�õİ汾��:" << "( Status: Using GLEW )" << glewGetString(GLEW_VERSION) << std::endl;
	unsigned char* glVersion;
	GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
	std::cout<<"GLʹ�õİ汾��:" << "( Status: Using GL )" << glVersion << std::endl;


	{ //�޶�������
		// ����λ�ø��������� 
		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f,  // 0 ���½Ƕ��� (100,100)
			200.0f, 100.0f, 1.0f, 0.0f,  // 1 ���ϽǶ��� (200,100)
			200.0f, 200.0f, 1.0f, 1.0f,  // 2 ���ϽǶ��� (200,200)
			100.0f, 200.0f, 0.0f, 1.0f   // 3 ���½Ƕ��� (100,200)
		};
		// ���������������������� 
		unsigned int indices[] = {
			0, 1, 2, // ȡ��positions����� 0��1��2
			2, 3, 0
		};

		/*
		���:�������ɫ(�ж���ɫ���������ɫ)��Ŀ�껺�������е���ɫ���
		glEnable/glDisable(����&�ر�) => glBlendFunc(ָ����ɫ����) => glBlendEquation(ָ�����ģʽ)
		glBlendEquation(mode) mode: src��dest�Ļ�Ϸ�ʽ(Ĭ��GL_FUNC_ADD, ����)
		*/
		GLCall(glEnable(GL_BLEND));//���û��(Ĭ�ϲ�������)
		/*
		glBlendFunc(src, dest) ָ����ɫ����
		src ָ�������ɫ(RGBA)���ӵļ��㷽ʽ, Ĭ��ΪGL_ONE
		dest ָ��Ŀ����ɫ���ӵļ��㷽ʽ, Ĭ��ΪGL_ZERO
		GL_SRC_ALPHA ��Ϊsrc��alphaΪ0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
		RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
		*/
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//ʹ�û��
		

		VertexArray va;//ʹ�ö�������

		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		//��������������
		IndexBuffer ib(indices, 6);

		// ��:0 �� ��:1280 �� ��:0 ����:720
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		// ���λ��          ��ͼ���� 
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));//�����ƶ�100
		// ģ�;��� ����λ�� 
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));//�����ƶ�200�������ƶ�200
		// ģ����ͼͶӰ����
		glm::mat4 mvp = proj * view * model;  


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP",mvp);



		Texture texture("res/textures/avater.png");
		texture.Bind();//������Ĭ��ֵ��0
		shader.SetUniform1i("u_Texture", 0);


		//��������еĶ���
		va.UnBind();
		shader.UnBind();
		vb.UnBind();
		ib.UnBind();



		Renderer renderer;

		ImGui::CreateContext();//����ImGui������
		ImGui_ImplGlfw_InitForOpenGL(window, true);//����GLFW����
		ImGui::StyleColorsDark();//ʹ��ImGui�ġ���ɫģʽ��


		
		const char* glsl_version = "#version 330";// ��Ҫָ��glsl�汾, Ҳ����shader�е�version
		ImGui_ImplOpenGL3_Init(glsl_version);//��ImGui��OpenGL��������
		glm::vec3 tranlation(200, 200, 0);

		float r = 0.0f;
		float increment = 0.005;


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			renderer.Clear();


			ImGui_ImplOpenGL3_NewFrame();//����ImGui��ʼһ���µ���Ⱦ֡
			ImGui_ImplGlfw_NewFrame();//����ImGui�������µ�GLFW�����¼�
			ImGui::NewFrame();//ImGui׼����ʼһ���µĽ���Ԫ�ز���֡

			glm::mat4 model = glm::translate(glm::mat4(1.0f), tranlation);
			glm::mat4 mvp = proj * view * model; // ģ����ͼͶӰ���� 

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp); 
			renderer.Draw(va,ib,shader);

			va.Bind();//�󶨶�������

			ib.Bind();

		/*
			if (r > 1.0f)
				increment = -0.005;
			if (r < 0.0f)
				increment = 0.005f;

			r += increment;
		*/
			{
			

				ImGui::Begin("ImGui");//��ʼһ����Ϊ"ImGui"�Ľ��沼�ֿ�
				ImGui::SliderFloat3("Tranlation", &tranlation.x, 0.0f, 1100.0f);//�����һ����Ϊ"Translation"�Ļ������ؼ�
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				

				ImGui::End();

				
			}

			ImGui::Render();//����ǰ��UI����֡��ȾΪһ��������UIͼ��
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//�����һ�����ɵ�UIͼ�����ݻ��Ƶ�OpenGL��������  


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