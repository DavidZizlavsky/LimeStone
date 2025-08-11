#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace LimeStone {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData);

	class Application {
	private:
		GLFWwindow* m_window = nullptr;
		VkInstance m_vkInstance = nullptr;
		VkDebugUtilsMessengerEXT m_vkDebugMessenger = nullptr;
		VkPhysicalDevice m_vkPhysicalDevice = nullptr;
		VkDevice m_vkDevice = nullptr;
		VkQueue m_vkGraphicsQueue = nullptr;

		void initVulkan();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	public:
		Application();
		~Application();
	};
}