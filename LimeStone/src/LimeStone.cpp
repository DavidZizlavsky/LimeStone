#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

#include "LimeStone.h"

namespace LimeStone {
	// Enable validation layers only in debug mode
#ifdef DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	
	// Default window size
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 500;

	// Application constants
	const char* APP_NAME = "LimeStone - application";
	const uint32_t APP_VERSION = VK_MAKE_VERSION(1, 0, 0);
	const char* ENGINE_NAME = "LimeStone";
	const uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);
	const uint32_t VULKAN_API_VERSION = VK_API_VERSION_1_0;

	Application::Application() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(WIDTH, HEIGHT, APP_NAME, nullptr, nullptr);

		initVulkan();

		while (!glfwWindowShouldClose(m_window)) {
			glfwPollEvents();
		}
	}

	void Application::initVulkan() {
		std::cout << "Initializing Vulkan!" << std::endl;

		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("Requested validation layers are not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = APP_NAME;
		appInfo.applicationVersion = APP_VERSION;
		appInfo.pEngineName = ENGINE_NAME;
		appInfo.engineVersion = ENGINE_VERSION;
		appInfo.apiVersion = VULKAN_API_VERSION;
		
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		
		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create VkInstance!");
		}

		std::cout << "Vulkan instance created successfully!" << std::endl;
	}
	
	Application::~Application() {
		vkDestroyInstance(m_vkInstance, nullptr);

		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	bool Application::checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}
		return true;
	}
}