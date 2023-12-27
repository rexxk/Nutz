workspace "Nutz"
	architecture "x86_64"
	configurations { "Debug", "Release" }
	startproject "Sandbox"

group "Core"

project "NutzLib"
	location "NutzLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
		
	files
	{
		"%{prj.location}/src/**.h", "%{prj.location}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.location}/src",
		"%{prj.location}/vendor/spdlog/include",
		"%{prj.location}/vendor/glfw/include",
	}

	pchheader "nutz_pch.h"
	pchsource "%{prj.location}/src/nutz_pch.cpp"
	
	defines "_CRT_SECURE_NO_WARNINGS"

	filter { "configurations:Debug" }
		defines "DEBUG"
		symbols "On"
	
	filter { "configurations:Release" }
		defines "NDEBUG"
		optimize "On"

	filter { "system:Windows" }

	filter { "system:Linux" }


group "Dependencies"

	include "NutzLib/vendor/glfw"

group "Tools"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	
	files
	{
		"%{prj.location}/src/**.h", "%{prj.location}/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/NutzLib/src",
		"%{wks.location}/NutzLib/vendor/spdlog/include"
	}
	
	libdirs
	{
		"%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/NutzLib",
		"%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/glfw"
	}
	
	links
	{
		"NutzLib"
	}
	
	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"
	
	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"

	filter { "system:Windows" }

	filter { "system:Linux" }
		links
		{
			"glfw"
		}
	
	
