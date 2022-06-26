include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "MoonEngine"
    architecture "x86_64"
    startproject "Editor"

    configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

    solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Engine/vendor/Glad"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/ImGui"
	include "Engine/vendor/yaml"
group ""

group "Tools"
    include "Editor"
group ""

include "Engine"
include "Sandbox"
