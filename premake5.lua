workspace "Nutz"
	architecture "x86_64"
	configurations { "Debug", "Release" }
	startproject "NutEditor"

group "Core"

	include "NutzLib"

group "Dependencies"

	include "NutzLib/vendor/glfw"

group "Tools"

--	include "Sandbox"
	include "NutEditor"
	
