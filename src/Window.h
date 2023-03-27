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

		Window(const Window& c) = delete;
		Window &operator=(const Window& c) =delete;
			
		bool ShouldCloseWindow() const { return glfwWindowShouldClose(m_window); }
		void OnUpdate();
		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		VkExtent2D getExtent() {return {m_width,m_height}; }
		bool wasWindowResized() {return m_frameBufferResized;}
		void resetWindowResizedFlag() {m_frameBufferResized = false;}
		GLFWwindow* getGLFWwindow() const { return m_window; }

	private:
		void Init();
		void CleanUp();
		static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
	private:
		GLFWwindow* m_window;
		std::string m_title;
		uint32_t m_width, m_height;
		bool m_frameBufferResized = false;

	};
}
