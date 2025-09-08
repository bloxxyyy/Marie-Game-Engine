workspace "MarieEngine"
    architecture "x64"
    startproject "Sandbox"
    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- =========================
-- Engine project
-- =========================
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

        -- GLAD / STB
        "vendor/src/GLAD/glad.c",
        "vendor/src/STB/stb_image.cpp",

        -- ImGui core
        "vendor/imgui/*.cpp",

        -- ImGui backends (GLFW + OpenGL3)
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",

        -- ImGui stdlib helper
        "vendor/imgui/misc/cpp/imgui_stdlib.cpp",

        -- Engine assets
        "%{prj.name}/Shaders/**.vert",
        "%{prj.name}/Shaders/**.frag",
        "%{prj.name}/Textures/**.*"
    }

    includedirs {
        "%{prj.name}/include",
        "vendor/include",
        "vendor/include/STB",
        "vendor/include/GLM",
        "vendor/imgui",
        "vendor/imgui/backends",
        "vendor/imgui/misc/cpp"
    }

    libdirs {
        "vendor/lib/GLFW"
    }

    links {
        "glfw3",
        "opengl32"
    }

    vpaths {
        ["Header Files"] = {"include/**.h"},
        ["Source Files"] = {"src/**.cpp"},
        ["Shader Files"] = {"Shaders/**.vert", "Shaders/**.frag"},
        ["External/ImGui"] = {
            "vendor/imgui/*.cpp",
            "vendor/imgui/backends/*.cpp",
            "vendor/imgui/misc/cpp/*.cpp",
            "vendor/imgui/*.h",
            "vendor/imgui/backends/*.h",
            "vendor/imgui/misc/cpp/*.h"
        }
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- =========================
-- Sandbox project
-- =========================
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
        "vendor/include",
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

-- =========================
-- Solution-level files (Visual Studio)
-- =========================
project "SolutionItems"
    kind "Utility"
    language "C++"
    location "."
    files {
        ".gitignore",
        "Scripts/build.ps1",
        "premake5.lua",
        "vendor/imgui/misc/debuggers/imgui.natvis",
        "vendor/imgui/misc/debuggers/imgui.natstepfilter"
    }
