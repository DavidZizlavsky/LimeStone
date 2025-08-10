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

		std::vector<const char*> extensions = getRequiredExtensions();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());;
		createInfo.ppEnabledExtensionNames = extensions.data();
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

		if (enableValidationLayers) {
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = 
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;
			debugCreateInfo.pUserData = nullptr;

			auto funcCreateDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugUtilsMessengerEXT");
			if (funcCreateDebugMessenger == nullptr) {
				std::cerr << "Debug Utils Messenger function could not be loaded!" << std::endl;
				throw std::runtime_error("Failed to initialized Debug Utils Messenger!");
			}

			VkResult debugMessengerResult = funcCreateDebugMessenger(m_vkInstance, &debugCreateInfo, nullptr, &m_vkDebugMessenger);
			if (debugMessengerResult != VK_SUCCESS) {
				std::cerr << "Failed to create debug messenger!" << std::endl;
				throw std::runtime_error("Failed to initialized Debug Utils Messenger!");
			}
		}

		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(m_vkInstance, &physicalDeviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(m_vkInstance, &physicalDeviceCount, physicalDevices.data());
		if (physicalDeviceCount == 0) {
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");
		}
		std::cout << "Physical devices available: " << std::endl;
		uint32_t maxMemory = 0;
		for (const VkPhysicalDevice& device : physicalDevices) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			uint32_t memory = deviceProperties.limits.maxMemoryAllocationCount;
			std::cout << "- " << deviceProperties.deviceName << " - " << memory << std::endl;
			if (memory > maxMemory) {
				maxMemory = memory;
				m_vkPhysicalDevice = device;
			}
		}

		if (m_vkPhysicalDevice == nullptr) {
			throw std::runtime_error("Failed to find a valid GPU!");
		}

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &deviceProperties);
		std::cout << "Selected: " << deviceProperties.deviceName << std::endl;
	}
	
	Application::~Application() {
		if (enableValidationLayers) {
			auto funcDestroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT");
			if (funcDestroyDebugMessenger != nullptr) {
				funcDestroyDebugMessenger(m_vkInstance, m_vkDebugMessenger, nullptr);
			}
		}

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

	std::vector<const char*> Application::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	QueueFamilyIndices Application::findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			i++;
		}

		return indices;
	}
}