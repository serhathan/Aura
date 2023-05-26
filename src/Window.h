#pragma once
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace Aura {

	static bool m_isWindowMinimized;

	class Window
	{
	public:
		Window(std::string title, uint32_t width, uint32_t height);
		~Window();

		Window(const Window& c) = delete;
		Window &operator=(const Window& c) =delete;
			
		bool ShouldCloseWindow() const { return glfwWindowShouldClose(m_window); }
		void OnUpdate();
		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		VkExtent2D GetExtent() {return {m_width,m_height}; }
		bool WasWindowResized() {return m_frameBufferResized;}
		void ResetWindowResizedFlag() {m_frameBufferResized = false;}
		GLFWwindow* GetGLFWWindow() const { return m_window; }

	private:
		void Init();
		void CleanUp();
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
		static void WindowIconifyCallback(GLFWwindow* window, int iconified);

	private:
		GLFWwindow* m_window;
		std::string m_title;
		uint32_t m_width, m_height;
		bool m_frameBufferResized = false;

	};
}
