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
		"%{prj.location}/src"
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
		"%{wks.location}/NutzLib/src"
	}
	
	libdirs
	{
		"%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/NutzLib"
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
