include "Dependencies.lua"

workspace "MoonEngine"
    architecture "x86_64"
    startproject "Sandbox"

    configurations 
	{
		"Debug",
		"Release",
		"Dist",
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
    include "Engine/vendor/Glad"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/ImGui"
group ""


project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "mepch.h"
    pchsource "Engine/src/mepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
	}

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",

        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
	{
		"Glad",
		"GLFW",
		"ImGui",
		"opengl32.lib",
	}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ME_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
		defines "ME_DEBUG"
		runtime "Debug"
		symbols "on"
        
    filter "configurations:Release"
		defines "ME_RELEASE"
		runtime "Release"
		optimize "on"
        
    filter "configurations:Dist"
		defines "ME_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    
    includedirs
    {
        "Engine/src",
        "Engine/vendor/spdlog/include",

        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "Engine"
    }

	filter "system:windows"
		systemversion "latest"

        defines
        {
            "ME_PLATFORM_WINDOWS",
        }

	filter "configurations:Debug"
		defines "FN_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "FN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FN_DIST"
		runtime "Release"
		optimize "on"
