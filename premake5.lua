workspace "MarieEngine"
    architecture "x64"
    startproject "Sandbox"
    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Engine project
project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h",
        "vendor/src/GLAD/glad.c",
        "vendor/src/STB/stb_image.cpp",
        "%{prj.name}/Shaders/**.vert",
        "%{prj.name}/Shaders/**.frag",
        "%{prj.name}/Textures/**.*"
    }

    includedirs {
        "%{prj.name}/include",
        "vendor/include",
        "vendor/include/STB",
    }

    libdirs {
        "vendor/lib/GLFW"
    }

    links {
        "glfw3",
        "opengl32"
    }

    vpaths {
        ["Header Files"] = {"**.h"},
        ["Source Files"] = {"**.cpp"},
        ["Shader Files"] = {"Shaders/**.vert", "Shaders/**.frag"}
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Sandbox project
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h"
    }

    includedirs {
        "Engine/include",
        "vendor/include"
    }

    links {
        "Engine"
    }

    vpaths {
        ["Header Files"] = {"**.h"},
        ["Source Files"] = {"**.cpp"}
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Solution-level files
project "SolutionItems"
    kind "Utility" 
    language "C++"
    location "."
    files {
        ".gitignore",
        "Scripts/build.ps1",
        "premake5.lua"
        -- add other root-level files here if needed (README.md, LICENSE, etc.)
    }
