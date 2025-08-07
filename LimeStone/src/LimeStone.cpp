#include "LimeStone.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace LimeStone 
{
	void printTest()
	{
		std::cout << "Hello from LimeStone!" << std::endl;
	}

	void showWindow()
	{
		glfwInit();
		GLFWwindow* window = glfwCreateWindow(500, 500, "Test", nullptr, nullptr);
		
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "VulkanInitTest";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "NoEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		VkInstance instance;
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (result != VK_SUCCESS) {
			std::cerr << "Failed to create Vulkan instance! Error code: " << result << std::endl;
		}

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			std::cerr << "No Vulkan-compatible GPUs found!" << std::endl;
		}
		else {
			std::cout << "Found " << deviceCount << " Vulkan-capable device(s):" << std::endl;
		}

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
		vkDestroyInstance(instance, nullptr);
		glfwTerminate();
	}
}