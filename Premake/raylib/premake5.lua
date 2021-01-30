dofile ("raylib_premake5.lua")
dofile ("examples_premake5.lua")

workspace "raylib"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Debug.DLL"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter "configurations:Release.DLL"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"
		
	targetdir "bin/%{cfg.buildcfg}/"
		
raylib()
examples()