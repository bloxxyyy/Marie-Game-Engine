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
        "%{prj.name}/include/**.h"
    }

    includedirs {
        "%{prj.name}/include",
        "vendor"
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
        "vendor"
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
        ".gitignore"
        -- add other root-level files here if needed (README.md, LICENSE, etc.)
    }
