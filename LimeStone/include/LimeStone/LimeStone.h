#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>

namespace LimeStone {
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

		void initVulkan();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		
	public:
		Application();
		~Application();
	};
}