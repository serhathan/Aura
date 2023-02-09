#pragma once
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Aura {
	static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);


	class Window
	{
	public:
		Window(std::string title, uint32_t width, uint32_t height);
		~Window();

		void OnUpdate();
			

		bool ShouldCloseWindow() const { return glfwWindowShouldClose(m_Window); }
		uint32_t GetWidth() const  { return m_Width; }
		uint32_t GetHeight() const  { return m_Height; }
		std::string GetTitle() const { return m_Title; }
		GLFWwindow* GetWindow() const { return m_Window;}

	private:
		void CreateInstance();
		void CleanUp();


	private:
		GLFWwindow* m_Window;
		std::string m_Title;
		uint32_t m_Width, m_Height;
		bool frameBufferResized = false;


	};
}
