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

group "Tools"
    include "Editor"
group ""

include "Engine"
include "Sandbox"
