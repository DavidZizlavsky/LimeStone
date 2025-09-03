#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace LimeStone {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	static std::vector<char> readFile(const std::string& filename);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData);

	class Application {
	private:
		std::string m_exeDirPath;
		GLFWwindow* m_window = nullptr;
		VkInstance m_vkInstance = nullptr;
		VkDebugUtilsMessengerEXT m_vkDebugMessenger = nullptr;
		VkPhysicalDevice m_vkPhysicalDevice = nullptr;
		VkDevice m_vkDevice = nullptr;
		VkQueue m_vkGraphicsQueue = nullptr;
		VkQueue m_vkPresentQueue = nullptr;
		VkSurfaceKHR m_vkSurface = nullptr;
		VkSwapchainKHR m_vkSwapchain = nullptr;
		std::vector<VkImage> m_vkSwapchainImages;
		VkFormat m_vkSwapchainImageFormat;
		VkExtent2D m_vkSwapchainExtent;
		VkPipelineLayout m_vkPipelineLayout = nullptr;
		std::vector<VkImageView> m_vkSwapchainImageViews;
		VkRenderPass m_vkRenderPass = nullptr;
		VkPipeline m_vkGraphicsPipeline = nullptr;

		void initVulkan();
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createGraphicsPipeline();

		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice device) const;
		SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice device) const;
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkShaderModule createShaderModule(const std::vector<char>& code) const;
	public:
		Application(std::string executableDir);
		~Application();
	};
}