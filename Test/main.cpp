#include "NativeHost.h"

GLuint vertexBuffer, vertexShader, fragmentShader, shaderProgram;

static float s_Vertices[] =
{
	-0.6f, -0.4f,
	 0.6f, -0.4f,
	 0.f,  0.6f,
};

static const char* s_VertexShaderText = R"(
#version 330 core

layout(location = 0) in vec2 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

static const char* s_FragmentShaderText = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5, 0.5, 0.5, 1.0);
}
)";

void OnUpdate(const float timestep)
{
	glUseProgram(shaderProgram);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON)
{
}

NativeHostApp* CreateNativeHostApp()
{
	// Create app first to load OpenGL
	NativeHostApp* app = new NativeHostApp(OnUpdate, OnMsg);

	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferData(vertexBuffer, sizeof(s_Vertices), &s_Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &s_VertexShaderText, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &s_FragmentShaderText, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	return app;
}