#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

//������ѡ����Shader��Bufferȥ����ĳ�������Σ�OpenGL��һ��״̬��

//�������� Vertex Attribute �� shader
/*
Vertex Attribute:

opengl pipeline �Ĺ�����ʽ���ṩͼ�����͸����ݣ�Ȼ��洢��GPU�ϵ��ڴ���ڴ��������������Ҫ���Ƶ��������ݣ�

Ȼ������ʹ��shader����GPU��ִ�е�һ�ֳ��򣩶�ȡ�ⲿ�����ݣ�Ȼ������Ļ����ʾ����

�д����Ե��ǣ�����ʵ���ϻ���ͼ��ʹ�õ���һ��Vertex buffer ���洢��GPU�ϵ�һ�����ڴ�� buffer ��

��shader ʵ���Ͽ�ʼ��ȡ Vertex buffer ʱ������Ҫ֪�� buffer �Ĳ��֣� buffer ������ʲô��

������ǲ�˵�������ֻ�Ǻ�c++����������ûʲô����



glVertexAttribPointer() ������

stride: the amount of bytes between each vertex 12 for coordinate(index1), 8 for texture(index2), 12 for normal(index3)(bytes) so the stride is 32 bytes

pointer: ָ�����Ե�ָ�� coordinate offset = 0 ,texture offset = 12, normal offset = 20

*/

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	
	float positions[6] = { 
		-0.5f, -0.5f, 		 
		0.0f,  0.5f, 		 
		0.5f, -0.5f };  	
	unsigned int buffer; 	
	glGenBuffers(1, &buffer);//create an ID :buffer  	
							 
	//���� PS һ����ָ�����ϻ��� 	
	glBindBuffer(GL_ARRAY_BUFFER, buffer);  	
	//specify the buffer 	
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//!!!!! REMEMBER!!!!!!!
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//2: component count
															//stride ʵ������һ���㣨����˵һ��2ά��������������ɣ����������������� float ���ͳ��ȣ�

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/*
		glBegin(GL_TRIANGLES);
		glVertex2d(-0.5f, -0.5f);
		glVertex2d(0.0f, 0.5f);
		glVertex2d(0.5f, -0.5f);
		glEnd();
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);//��buffer������鿪ʼdraw��һ��3���

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}