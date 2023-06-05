
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
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	unsigned char* glVersion;
	GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
	std::cout << "Status: Using GL " << glVersion << std::endl;


	{ //�޶�������
		// ����λ�ø��������� 
		float positions[] = {
			-0.5f, -0.5f, // 0
			0.5f, -0.5f,  // 1
			0.5f, 0.5f,   // 2
			-0.5f, 0.5f,  // 3
		};
		// ���������������������� 
		unsigned int indices[] = {
			0, 1, 2, // ȡ��positions����� 0��1��2
			2, 3, 0
		};


		VertexArray va;//ʹ�ö�������

		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		//��������������
		IndexBuffer ib(indices, 6);


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		//��������еĶ���
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

			va.Bind();//�󶨶�������

			ib.Bind();



			if (r > 1.0f)
				increment = -0.005;
			if (r < 0.0f)
				increment = 0.005f;

			r += increment;
			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));//������룬�����еõ��������Ϣ

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//��ȷ����



			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}