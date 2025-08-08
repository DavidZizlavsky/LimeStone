#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>

#include "LimeStone.h"

namespace LimeStone {
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 500;

	Application::Application() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(WIDTH, HEIGHT, "LimeStone - application", nullptr, nullptr);

		while (!glfwWindowShouldClose(m_window)) {
			glfwPollEvents();
		}
	}

	Application::~Application() {
		glfwDestroyWindow(m_window);

		glfwTerminate();
	}
}