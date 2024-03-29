project "Moon-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mepch.h"
    pchsource "src/private/mepch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",

		"vendor/FastNoise/**.h",
		"vendor/FastNoise/**.cpp",

        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp",

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
        "src/public",
        "resource",

        "%{IncludeDir.Box2D}",
        "%{IncludeDir.entt}",
		"%{IncludeDir.FastNoise}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.yaml}",
    }

    links
	{
		"Box2D",
		"Glad",
		"GLFW",
		"ImGui",
        "msdf-atlas-gen",
		"yaml-cpp",
		"opengl32.lib",
	}

	filter "files:vendor/ImGuizmo/**.cpp or files:vendor/FastNoise/**.cpp"
	flags { "NoPCH" }

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

		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
		}

    filter "configurations:Release"
		defines "ME_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}",
		}

    filter "configurations:Dist"
		defines "ME_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}",
		}
