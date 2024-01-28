project "NutzLib"
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
		"%{prj.location}/vendor/stb",
		"$(VULKAN_SDK)/Include"
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

		links
		{
			"$(VULKAN_SDK)/Lib/vulkan-1.lib",
		}

		defines
		{
			"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
			"GLFW_EXPOSE_NATIVE_WIN32",
		}

		filter { "configurations:Debug" }

			links
			{
				"$(VULKAN_SDK)/Lib/glslangd.lib",
				"$(VULKAN_SDK)/Lib/MachineIndependentd.lib",
				"$(VULKAN_SDK)/Lib/GenericCodeGend.lib",
				"$(VULKAN_SDK)/Lib/OGLCompilerd.lib",
				"$(VULKAN_SDK)/Lib/SPIRVd.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-cross-glsld.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-Toolsd.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-Tools-optd.lib",
			}

		filter { "configurations:Release" }

			links
			{
				"$(VULKAN_SDK)/Lib/glslang.lib",
				"$(VULKAN_SDK)/Lib/MachineIndependent.lib",
				"$(VULKAN_SDK)/Lib/GenericCodeGen.lib",
				"$(VULKAN_SDK)/Lib/OGLCompiler.lib",
				"$(VULKAN_SDK)/Lib/SPIRV.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-cross-glsl.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-Tools-opt.lib",
				"$(VULKAN_SDK)/Lib/SPIRV-Tools-shared.lib",
			}

	filter { "system:Linux" }

		links
		{
			"vulkan",
		}

		defines
		{
			"GLFW_EXPOSE_NATIVE_X11",
		}