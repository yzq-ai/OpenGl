
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

#include "tests/TestClearColor.h"//����

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
		


		Renderer renderer;

		ImGui::CreateContext();//����ImGui������
		ImGui_ImplGlfw_InitForOpenGL(window, true);//����GLFW����
		ImGui::StyleColorsDark();//ʹ��ImGui�ġ���ɫģʽ��


		
		const char* glsl_version = "#version 330";// ��Ҫָ��GLSL�汾, Ҳ����shader�е�version
		ImGui_ImplOpenGL3_Init(glsl_version);//��ImGui��OpenGL��������

		test::TestClearColor test;

		glm::vec3 tranlationA(200, 200, 0);
		glm::vec3 tranlationB(400, 200, 0);

		
		while (!glfwWindowShouldClose(window)) {
			renderer.Clear();

			test.OnUpdate(0.0f);
            test.OnRender();

			ImGui_ImplOpenGL3_NewFrame();//����ImGui��ʼһ���µ���Ⱦ֡
			ImGui_ImplGlfw_NewFrame();//����ImGui�������µ�GLFW�����¼�
			ImGui::NewFrame();//ImGui׼����ʼһ���µĽ���Ԫ�ز���֡
			
			{
				test.OnImGuiRender();
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