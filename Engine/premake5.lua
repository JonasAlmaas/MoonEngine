project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "mepch.h"
    pchsource "src/mepch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",

        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
	}

    includedirs
    {
        "src",
        "resource",

        "%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb_image}",
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
            "ME_PLATFORM_WINDOWS"
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
