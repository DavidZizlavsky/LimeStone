workspace "LimeStone"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "SampleApp"

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "LimeStone"
include "SampleApp"