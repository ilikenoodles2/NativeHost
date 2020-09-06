#include "Window.h"

#include "vendor/glad/include/glad/glad.h"

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	s_Logfile << message << std::endl;
}

static void ErrorCallback(int error, const char* description)
{
	s_Logfile << "Error: " << error << description << std::endl;
}

static void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	windowPtr->GetLogger()->Resize(width, height);
}

static void WindowCloseCallback(GLFWwindow* window)
{
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	windowPtr->~Window();
}

Window::Window(OnUpdate onUpdate)
	: m_IsOpen(false), m_OnUpdate(onUpdate), m_Logger(nullptr)
{
	// All initialization done in StartProcess(), because window
	// runs on a separate thread than the native host
}

Window::~Window()
{
	m_IsOpen = false;
}

void Window::StartProcess(const bool& appReady, bool& windowInitialized)
{
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit())
		s_Logfile << "Failed to initialize GLFW" << std::endl;

	m_Window = glfwCreateWindow(640, 480, "Native host", NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		s_Logfile << "Failed to create window" << std::endl;
		return;
	}

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);
	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	m_IsOpen = true;
	s_Logfile << "Successfully created window" << std::endl;

	SetContext(true);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
		s_Logfile << "Failed to initialize glad" << std::endl;
	glfwSwapInterval(1);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	s_Logfile << "Successfully initialized glad" << std::endl;

	Logger logger;
	m_Logger = &logger;
	m_Logger->Init(m_Window);

	SetContext(false);

	windowInitialized = true;
	while(!appReady) {}

	SetContext(true);
	static float lastFrameTime = (float)glfwGetTime();

	while (m_IsOpen)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		float time = (float)glfwGetTime();
		float timestep = time - lastFrameTime;
		lastFrameTime = time;

		m_OnUpdate(timestep);
		m_Logger->Update(timestep);

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	m_Logger->Shutdown();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
	s_Logfile << "Terminated glfw" << std::endl;
}

void Window::SetContext(bool thisThread)
{
	glfwMakeContextCurrent(thisThread ? m_Window : NULL);
}