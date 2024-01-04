project "NutEditor"
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
		"NutzLib",
        "glfw"
	}
	
	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"
	
	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"

	filter { "system:Windows" }
