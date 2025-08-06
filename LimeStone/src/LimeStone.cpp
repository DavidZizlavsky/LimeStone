#include "LimeStone.h"
#include <iostream>
#include <GLFW/glfw3.h>

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
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
		glfwTerminate();
	}
}