include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "MoonEngine"
    architecture "x86_64"
    startproject "Moon-Editor"

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

group "Core"
	include "Moon-Engine"
group ""

group "Tools"
	include "Moon-Editor"
group ""

group "Dependencies"
    include "Moon-Engine/vendor/Box2D"
    include "Moon-Engine/vendor/Glad"
    include "Moon-Engine/vendor/GLFW"
    include "Moon-Engine/vendor/ImGui"
	include "Moon-Engine/vendor/msdf-atlas-gen"
	include "Moon-Engine/vendor/yaml"
group ""
