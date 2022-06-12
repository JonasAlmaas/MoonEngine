project "Sandbox"
    kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

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

        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "Engine"
    }

	filter "system:windows"
		systemversion "latest"

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
