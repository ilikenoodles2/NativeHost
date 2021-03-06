#include "Logger.h"

#include "vendor/imgui/imgui/imgui.h"
#include "vendor/imgui/imgui/examples/imgui_impl_glfw.h"
#include "vendor/imgui/imgui/examples/imgui_impl_opengl3.h"

void Logger::Init(void* window)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

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
	ImGui::Begin("Logger", 0, ImGuiWindowFlags_NoResize);
	{
		std::lock_guard<std::mutex> l(m_Mutex);
		if (ImGui::Button("Clear"))
			m_Buffer.clear();

		static bool enableHostLogs, enableUserLogs;
		ImGui::SameLine();
		ImGui::Checkbox("HostLogs", &enableHostLogs);
		ImGui::SameLine();
		ImGui::Checkbox("UserLogs", &enableUserLogs);

		ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		{
			for (const auto& msg : m_Buffer)
			{
				if(((msg.second & OriginMask) && enableHostLogs)
					|| (!(msg.second & OriginMask) && enableUserLogs))
					ImGui::Text(msg.first.c_str(), msg.second);
			}

			if (m_ShouldScrollToBottom)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
				m_ShouldScrollToBottom = false;
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Logger::Resize(int width, int height)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)width, (float)height);
}

void Logger::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
