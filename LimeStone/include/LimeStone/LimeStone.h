#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace LimeStone {
	class Application {
	private:
		GLFWwindow* m_window = nullptr;
		VkInstance m_vkInstance = nullptr;

		void initVulkan();
		bool checkValidationLayerSupport();
	public:
		Application();
		~Application();
	};
}