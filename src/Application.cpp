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
//������ɫ��Դ��
static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath); /* ����û�ж��ļ��Ƿ��������� is_open */
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) { /* �ҵ�#shader��� */
			if (line.find("vertex") != std::string::npos) { /* ������ɫ����� */
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) { /* Ƭ����ɫ����� */
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}
//������ɫ��
static unsigned int CompileShader(unsigned int type,const std::string& source) {
	
	unsigned int id = glCreateShader(type); /* ������Ӧ���͵���ɫ�� */
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); /* ������ɫ��Դ�� */
	glCompileShader(id); /* ������ɫ�� */


	//������
	int result;
	glGetShaderiv(id,GL_COMPILE_STATUS,&result);
	if (result == GL_FALSE)
 	{
		int length;
		glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
		char* message = (char*)_malloca(length*sizeof(char));//��ջ�϶�̬����䳤���飬alloca����Ҫfree�������������ʱ��̬�ͷ��ڴ�
		glGetShaderInfoLog(id,length,&length,message);
		std::cout << (type==GL_VERTEX_SHADER ?"����":"Ƭ��") << " ��ɫ������ʧ�ܣ�(Fialed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!)" << std::endl;
		std::cout << message << std::endl;
	
		glDeleteShader(id);
		return 0;
	}
	std::cout << (type == GL_VERTEX_SHADER ? "����" : "Ƭ��") << "��ɫ������ɹ���(Suffculty to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!)" << std::endl;
	return id;
}
//������ɫ��
static unsigned int CreateShader(const std::string& veterxShader,const std::string& fragementShader) 
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,veterxShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragementShader);

	//������ɫ��
	glAttachShader(program,vs);
	glAttachShader(program,fs);
	//������ɫ����������
	glLinkProgram(program);
	//�ڳ�����ִ����֤
	glValidateProgram(program);

	glDeleteShader(vs);//ɾ��������ɫ��
	glDeleteShader(fs);//ɾ��Ƭ����ɫ��

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

	//OpenGL�����ɵ����ж���������һ��Ψһ�ı�ʶ ---> һ������

	 /* ����λ�ø��������� */
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};
	unsigned int buffer; 	
	glGenBuffers(1, &buffer);//����һ��������,&buffer��ʾbuffer��idֵ
							 
	//���� PS һ����ָ�����ϻ��� 	
	glBindBuffer(GL_ARRAY_BUFFER, buffer);  	//��buffer����ʾ����ʹ��buffer
	//specify the buffer 	
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);//GL_STATIC_DRAW��ʾʹ�þ�̬���ƣ�һ���޸Ķ��ʹ��

	
	//�����ȵ��ô˺����ſ�ʹ�ö������ԣ���ΪOpenGL��״̬���ø������ú�����״̬
	glEnableVertexAttribArray(0);

	//��������
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//2: component count
															//stride ʵ������һ��ƫ���������ƽṹ��Ķ��ʵ���е�һ��ʵ�����ڴ�ռ䣨����˵һ��2ά��������������ɣ����������������� float ���ͳ��ȣ�
		
	 //���ļ��н�����ɫ��Դ�� 
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader); // ʹ����ɫ������ 

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); // ����

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader); /* ɾ����ɫ������ */
	glfwTerminate();
	return 0;
}