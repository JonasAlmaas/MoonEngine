
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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"

group "Dependencies"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/Glad"
group ""

project "Engine"
    location "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "mepch.h"
    pchsource "Engine/src/mepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ME_PLATFORM_WINDOWS",
            "ME_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
		defines "ME_DEBUG"
		runtime "Debug"
        buildoptions "/MDd"
		symbols "on"
        
        defines
        {
            "ME_ENABLE_ASSERTS"
        }
        
    filter "configurations:Release"
		defines "ME_RELEASE"
		runtime "Release"
        buildoptions "/MD"
		optimize "on"
        
    filter "configurations:Dist"
		defines "ME_DIST"
		runtime "Release"
        buildoptions "/MD"
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
        buildoptions "/MDd"
		symbols "on"
	
	filter "configurations:Release"
		defines "FN_RELEASE"
		runtime "Release"
        buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "FN_DIST"
		runtime "Release"
        buildoptions "/MD"
		optimize "on"
