#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak() //����: �������ж� 

// ��б�ܺ��治���пո�,�ᱻ������ʶ���ת���ַ� 
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)
//�������
static void GLClearError()
{
	// ѭ����ȡ����:���������ǰ�ĵĴ��� 
	while (glGetError() != GL_NO_ERROR);
}
//��־�ص�
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" <<std::hex<< error << "): "
			<< function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}


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
	
	unsigned int id;
	// ���������� 
	GLCall(id = glCreateShader(type)); // ������Ӧ���͵���ɫ�� 
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // ������ɫ��Դ�� 
	GLCall(glCompileShader(id)); // ������ɫ�� 


	//���������
	int result;
	GLCall(glGetShaderiv(id,GL_COMPILE_STATUS,&result));
	if (result == GL_FALSE)
 	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // ��ȡ��־����
		char* message = (char*)_malloca(length*sizeof(char));//��ջ�϶�̬����䳤���飬alloca����Ҫfree�������������ʱ��̬�ͷ��ڴ�
		GLCall(glGetShaderInfoLog(id,length,&length,message));// ��ȡ��־��Ϣ
		std::cout << (type==GL_VERTEX_SHADER ?"����":"Ƭ��") << " ��ɫ������ʧ�ܣ�(Fialed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!)" << std::endl;
		std::cout << message << std::endl;
	
		GLCall(glDeleteShader(id));
		return 0;
	}
	std::cout << (type == GL_VERTEX_SHADER ? "����" : "Ƭ��") << "��ɫ������ɹ���(Succed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!)" << std::endl;
	return id;
}
//������ɫ��
static unsigned int CreateShader(const std::string& veterxShader,const std::string& fragementShader) 
{
	unsigned int program;
	GLCall(program = glCreateProgram()); // �������� 
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,veterxShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragementShader);

	//������ɫ��
	GLCall(glAttachShader(program,vs));
	GLCall(glAttachShader(program,fs));
	GLCall(glLinkProgram(program));//������ɫ����������
	GLCall(glValidateProgram(program));//�ڳ�����ִ����֤

	GLCall(glDeleteShader(vs));//ɾ��������ɫ��
	GLCall(glDeleteShader(fs));//ɾ��Ƭ����ɫ��

	return program;
}

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

	//OpenGL�����ɵ����ж���������һ��Ψһ�ı�ʶ ---> һ������

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


	unsigned int vao; // ���涥���������ID 
	GLCall(glGenVertexArrays(1, &vao)); // ���ɶ������� 
	GLCall(glBindVertexArray(vao)); // �󶨶�������




	unsigned int buffer; 	
	
	GLCall(glGenBuffers(1, &buffer)); // ���ɻ����� 
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // �󶨻����� 
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); // ���û��������� 
	
	
	
	GLCall(glEnableVertexAttribArray(0)); // ���������-����0-λ�� 
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // ���ö�������-����0 
	

	//��������������
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));



	 //���ļ��н�����ɫ��Դ�� 
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader); // ʹ����ɫ������ 

	int location = glGetUniformLocation(shader,"u_Color");//�ñ����������ɫ����ʵ�ʱ�������ȫһ��,ʵ�ʻ�ȡ���Ǳ������ļ��е�λ��
	ASSERT(location!=-1);//���һ�µ�ǰ�ı����Ƿ񻹴��ڣ������ѱ�ʹ��
	
	//��������еĶ���
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	
	
	float r = 0.0f;
	float increment = 0.005;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));//����ɫ��
		GLCall( glUniform4f(location,r, 0.3f, 0.8f, 1.0f));//���ø�ͳһ����


		GLCall(glBindVertexArray(vao));



		GLCall(glBindBuffer(GL_ARRAY_BUFFER,buffer));//�󶨶��㻺����
		GLCall(glEnableVertexAttribArray(0)); // ���ö��㻺�������� 
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // ���ö�������-����0 
		GLCall(GL_ELEMENT_ARRAY_BUFFER,ibo);//������������
		
		

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

	GLCall(glDeleteProgram(shader)); // ɾ����ɫ������ 
	glfwTerminate();
	return 0;
}