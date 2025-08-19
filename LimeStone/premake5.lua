project "LimeStone"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "off"

    files { "**.h", "**.hpp", "**.cpp", "**.vert", "**.frag" }

    includedirs
    {
        "include/LimeStone",
        "$(VULKAN_SDK)/Include",
        "vendor/glfw-3.4.bin.WIN64/include"
    }

    links 
    {
        "$(VULKAN_SDK)/Lib/vulkan-1.lib",
        "vendor/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.lib"
    }

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }
        
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
        
    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"