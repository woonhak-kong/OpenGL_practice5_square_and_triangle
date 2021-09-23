#include <iostream>
#include <fstream>

// glew.h는 gl.h를 포함하기 전에 포함해야한다.
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

	// 1. 쉐이더 오브젝트를 생성한다.
	GLuint vertShader = setShader("vertex", "basic.vert");
	GLuint fragShader = setShader("fragment", "basic.frag");

	// 오브젝트 컴파일이 끝나면 OpenGL pipeline에 컴파일이 끝난 쉐이더를 등록 혹은 설치해야한다.

	// 1. 먼저 Program object를 생성한다.
	// 빈 프로그램 생성
	programHandle = glCreateProgram();
	if (0 == programHandle)
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}

	// 2. 쉐이더들을 프로그램에 붙인다.
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);

	// 3. 프로그램을 링크한다. 연결
	glLinkProgram(programHandle);

	// 4. 링크 상태 확인
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
	// 5. 만약 링크가 성공했다면 프로그램을 OpenGL pipeline에 설치 한다.
	else
	{
		glUseProgram(programHandle);
	}


	// 포지션과 색상을 저장하기 위한 버퍼를 생성한다.
	// Create the buffer objects
	GLuint vboHandles[4];
	// 버퍼 두개생성.
	glGenBuffers(4, vboHandles);
	GLuint positionBufferHandle1 = vboHandles[0];
	GLuint colorBufferHandle1 = vboHandles[1];

	GLuint positionBufferHandle2 = vboHandles[2];
	GLuint colorBufferHandle2 = vboHandles[3];


	// 각 버퍼의 데이타를 등록, 옮긴다.
	// Populate the position buffer
	// 포지션1 버퍼를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle1);
	// 포지션1 값을 등록한다.
	// 두번째 인자는 배열의 사이즈 이다. 세번째 인자는 해당 배열
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices1, GL_STATIC_DRAW);
	// Populate the color buffer
	// 색상1 버퍼를 바인드한다.
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle1);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors1, GL_STATIC_DRAW);

	// 포지션2 버퍼를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle2);
	// 포지션2 값을 등록한다.
	// 두번째 인자는 배열의 사이즈 이다. 세번째 인자는 해당 배열
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices2, GL_STATIC_DRAW);
	// Populate the color buffer
	// 색상2 버퍼를 바인드한다.
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle2);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors2, GL_STATIC_DRAW);


	// 버퍼 오브젝트롤 설정하였기 때문에, 이것들을 vertex array obejct(VAO) 에 묶는다.
	// VAO 생성한다. (전역변수로 vaoHandle 필요)
	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// 각 속성 인덱스의 generic vertex attribute 를 활성화 시킨다.
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // for Vertex position
	glEnableVertexAttribArray(1); // for Vertex color


	// 버퍼 오브젝트와 generic vertex attribute index들을 연결시킨다.
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle1);
	// 첫번째인자 - generic attribute index, 두번째인자 - 각 vertex의 요소수(여기서는 3)(1,2,3 or 4)
	// 세번째인자 - 각 요소의 데이타 타입, 네번째인자 - normalize 여부, 다섯번째 인자 - byte offset
	// 여섯번째인자 - 버퍼의 시작 offset(여기서는 포지션값밖에 없기때문에 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);



	///////////////////// 삼각형 바인드
	// vertextArray를 하나 더 만든다.
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// 각 속성 인덱스의 generic vertex attribute 를 활성화 시킨다.
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
	// 3번째 인자 - 버택스의 수
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(2);
	// 3번째 인자 - 버택스의 수
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//윈도우 사이즈 변경
	glutInitWindowSize(512, 512);

	// top-left corner 초기 포지션으로 초기화
	glutInitWindowPosition(0, 0);

	// 윈도우창 생성
	glutCreateWindow("Hello World");


	// glew를 초기화 해준다. opengl을 사용하기위해서
	GLenum err = glewInit();
	// glewInit()을 함으로써 모든 OpenGL라이브러리를 찾고 모든 사용가능한 함수포인터를 초기화한다.

	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));
	}

	// to compile shader
	init();

	glutDisplayFunc(display);


	// glut의 이벤트 프로세싱 loop을 시작.
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


	// 2. 쉐이더 소스코드를 쉐이더 오브젝트로 복사한다.
	const GLchar* shaderCode = loadShaderAsString(shaderName);
	// 소스 배열에 여러 소스코드를 담을 수 있다.
	// 배열에 소스코드를 담은후.
	const GLchar* codeArray[] = { shaderCode };
	// vertShader object로 codeArray를 복사한다.
	// 첫번째 인자는 쉐이더 오브젝트, 두번째 인자는 소스코드의 총 개수 여기서는 shaderCode 한개만 들어가서 1
	// 세번째 인자는 코드를 넣은 배열, 네번째 인자는 각 소스코드의 길이를 넣은 int배열이다 여기서는 null character를 넣어서 자동으로 확인되기때무에 NULL을 넣었다.
	glShaderSource(shaderObj, 1, codeArray, NULL);

	// 3. 쉐이더를 컴파일 한다.
	glCompileShader(shaderObj);


	// 4. 컴파일 완료 확인.
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