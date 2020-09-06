#include "Logger.h"

#include "vendor/imgui/imgui/imgui.h"
#include "vendor/imgui/imgui/examples/imgui_impl_glfw.h"
#include "vendor/imgui/imgui/examples/imgui_impl_opengl3.h"

Logger::~Logger()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Logger::Init(void* window)
{
	ImGui::CreateContext();
	ImGui::StyleColorsLight();

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void Logger::Update(const float timestep)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("Logger", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Logger::Resize(int width, int height)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)width, (float)height);
}
