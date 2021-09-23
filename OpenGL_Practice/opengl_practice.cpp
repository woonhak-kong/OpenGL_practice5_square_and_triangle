#include <iostream>
#include <fstream>

// glew.h�� gl.h�� �����ϱ� ���� �����ؾ��Ѵ�.
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


GLuint vaoHandle;
GLuint programHandle;
GLuint setShader(const char* shaderType, const char* shaderName);
char* loadShaderAsString(std::string fileName);


GLfloat vertices1[] = {
	-0.9f,  0.9f,  0.0f,
	0.9f,  0.9f,  0.0f,
	0.9f, -0.9f,  0.0f,
	-0.9f, -0.9f,  0.0f
};

GLfloat colors1[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};


GLfloat vertices2[] = {
	0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,
   -0.5f, 0.5f, 0.0f
};

GLfloat colors2[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

void init()
{

	// 1. ���̴� ������Ʈ�� �����Ѵ�.
	GLuint vertShader = setShader("vertex", "basic.vert");
	GLuint fragShader = setShader("fragment", "basic.frag");

	// ������Ʈ �������� ������ OpenGL pipeline�� �������� ���� ���̴��� ��� Ȥ�� ��ġ�ؾ��Ѵ�.

	// 1. ���� Program object�� �����Ѵ�.
	// �� ���α׷� ����
	programHandle = glCreateProgram();
	if (0 == programHandle)
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}

	// 2. ���̴����� ���α׷��� ���δ�.
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);

	// 3. ���α׷��� ��ũ�Ѵ�. ����
	glLinkProgram(programHandle);

	// 4. ��ũ ���� Ȯ��
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		fprintf(stderr, "Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
	}
	// 5. ���� ��ũ�� �����ߴٸ� ���α׷��� OpenGL pipeline�� ��ġ �Ѵ�.
	else
	{
		glUseProgram(programHandle);
	}


	// �����ǰ� ������ �����ϱ� ���� ���۸� �����Ѵ�.
	// Create the buffer objects
	GLuint vboHandles[4];
	// ���� �ΰ�����.
	glGenBuffers(4, vboHandles);
	GLuint positionBufferHandle1 = vboHandles[0];
	GLuint colorBufferHandle1 = vboHandles[1];

	GLuint positionBufferHandle2 = vboHandles[2];
	GLuint colorBufferHandle2 = vboHandles[3];


	// �� ������ ����Ÿ�� ���, �ű��.
	// Populate the position buffer
	// ������1 ���۸� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle1);
	// ������1 ���� ����Ѵ�.
	// �ι�° ���ڴ� �迭�� ������ �̴�. ����° ���ڴ� �ش� �迭
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices1, GL_STATIC_DRAW);
	// Populate the color buffer
	// ����1 ���۸� ���ε��Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle1);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors1, GL_STATIC_DRAW);

	// ������2 ���۸� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle2);
	// ������2 ���� ����Ѵ�.
	// �ι�° ���ڴ� �迭�� ������ �̴�. ����° ���ڴ� �ش� �迭
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices2, GL_STATIC_DRAW);
	// Populate the color buffer
	// ����2 ���۸� ���ε��Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle2);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors2, GL_STATIC_DRAW);


	// ���� ������Ʈ�� �����Ͽ��� ������, �̰͵��� vertex array obejct(VAO) �� ���´�.
	// VAO �����Ѵ�. (���������� vaoHandle �ʿ�)
	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// �� �Ӽ� �ε����� generic vertex attribute �� Ȱ��ȭ ��Ų��.
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // for Vertex position
	glEnableVertexAttribArray(1); // for Vertex color


	// ���� ������Ʈ�� generic vertex attribute index���� �����Ų��.
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle1);
	// ù��°���� - generic attribute index, �ι�°���� - �� vertex�� ��Ҽ�(���⼭�� 3)(1,2,3 or 4)
	// ����°���� - �� ����� ����Ÿ Ÿ��, �׹�°���� - normalize ����, �ټ���° ���� - byte offset
	// ������°���� - ������ ���� offset(���⼭�� �����ǰ��ۿ� ���⶧���� 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);



	///////////////////// �ﰢ�� ���ε�
	// vertextArray�� �ϳ� �� �����.
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// �� �Ӽ� �ε����� generic vertex attribute �� Ȱ��ȭ ��Ų��.
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // for Vertex position
	glEnableVertexAttribArray(1); // for Vertex color

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(1);
	// 3��° ���� - ���ý��� ��
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(2);
	// 3��° ���� - ���ý��� ��
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//������ ������ ����
	glutInitWindowSize(512, 512);

	// top-left corner �ʱ� ���������� �ʱ�ȭ
	glutInitWindowPosition(0, 0);

	// ������â ����
	glutCreateWindow("Hello World");


	// glew�� �ʱ�ȭ ���ش�. opengl�� ����ϱ����ؼ�
	GLenum err = glewInit();
	// glewInit()�� �����ν� ��� OpenGL���̺귯���� ã�� ��� ��밡���� �Լ������͸� �ʱ�ȭ�Ѵ�.

	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));
	}

	// to compile shader
	init();

	glutDisplayFunc(display);


	// glut�� �̺�Ʈ ���μ��� loop�� ����.
	glutMainLoop();

	return 0;
}



GLuint setShader(const char* shaderType, const char* shaderName)
{
	GLuint shaderObj;
	if (strcmp(shaderType, "vertex") == 0)
	{
		shaderObj = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (strcmp(shaderType, "fragment") == 0)
	{
		shaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	}


	if (0 == shaderObj)
	{
		fprintf(stderr, "Error creating shader obj.\n");
		exit(1);
	}


	// 2. ���̴� �ҽ��ڵ带 ���̴� ������Ʈ�� �����Ѵ�.
	const GLchar* shaderCode = loadShaderAsString(shaderName);
	// �ҽ� �迭�� ���� �ҽ��ڵ带 ���� �� �ִ�.
	// �迭�� �ҽ��ڵ带 ������.
	const GLchar* codeArray[] = { shaderCode };
	// vertShader object�� codeArray�� �����Ѵ�.
	// ù��° ���ڴ� ���̴� ������Ʈ, �ι�° ���ڴ� �ҽ��ڵ��� �� ���� ���⼭�� shaderCode �Ѱ��� ���� 1
	// ����° ���ڴ� �ڵ带 ���� �迭, �׹�° ���ڴ� �� �ҽ��ڵ��� ���̸� ���� int�迭�̴� ���⼭�� null character�� �־ �ڵ����� Ȯ�εǱ⶧���� NULL�� �־���.
	glShaderSource(shaderObj, 1, codeArray, NULL);

	// 3. ���̴��� ������ �Ѵ�.
	glCompileShader(shaderObj);


	// 4. ������ �Ϸ� Ȯ��.
	GLint result;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, "shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shaderObj, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			free(log);
		}
	}

	return shaderObj;
}

char* loadShaderAsString(std::string fileName)
{
	// Initialize input stream.
	std::ifstream inFile(fileName.c_str(), std::ios::binary);

	// Determine shader file length and reserve space to read it in.
	inFile.seekg(0, std::ios::end);
	int fileLength = inFile.tellg();
	char* fileContent = (char*)malloc((fileLength + 1) * sizeof(char));

	// Read in shader file, set last character to NUL, close input stream.
	inFile.seekg(0, std::ios::beg);
	inFile.read(fileContent, fileLength);
	fileContent[fileLength] = '\0';
	inFile.close();

	return fileContent;
}