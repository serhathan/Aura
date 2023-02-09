#include "Window.h"
#include"pch.h"

namespace Aura {

	Window::Window(std::string title, uint32_t width, uint32_t height)
		: m_Title(title),m_Width(width),m_Height(height)
	{
		CreateInstance();
	}
	Window::~Window()
	{
		CleanUp();
	}

	void Window::CreateInstance() 
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		glfwGetWindowUserPointer(m_Window);
		//glfwSetFramebufferSizeCallback(m_Window, FrameBufferResizeCallback);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}


	void Window::CleanUp()
	{
		glfwDestroyWindow(m_Window);

		glfwTerminate();
	}


	/*static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		app->frameBufferResized = true;
	}*/
}
