#include "Window.h"
#include"pch.h"

namespace Aura {

	Window::Window(std::string title, uint32_t width, uint32_t height)
		: m_title(title),m_width(width),m_height(height)
	{
		Init();
	}
	Window::~Window()
	{
		CleanUp();
	}

	void Window::Init() 
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		glfwGetWindowUserPointer(m_window);
		//glfwSetFramebufferSizeCallback(m_Window, FrameBufferResizeCallback);
		
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}

	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}




	void Window::CleanUp()
	{
		glfwDestroyWindow(m_window);

		glfwTerminate();
	}




	/*static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		app->frameBufferResized = true;
	}*/
}
