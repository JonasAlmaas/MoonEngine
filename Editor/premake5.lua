project "Editor"
    kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "aopch.h"
    pchsource "src/aopch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",

        "../Engine/src",
        "../Engine/vendor/spdlog/include",

        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
    }

    links
    {
        "Engine"
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
