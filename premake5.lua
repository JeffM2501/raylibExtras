workspace "RaylibExtras"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64"}
	defaultplatform "x64"
	
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
		
project "raylib"
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "./"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src", "raylib/src/external/glfw/include"}
	vpaths 
	{
		["Header Files"] = { "raylib/src/**.h"},
		["Source Files/*"] = {"raylib/src/**.c"},
	}
	files {"raylib/src/*.h", "raylib/src/*.c"}
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
		
project "rlExtrasC"
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		links {"raylib"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "./rlExtrasC/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src","rlExtrasC"}
	vpaths 
	{
		["Header Files"] = { "rlExtrasC/**.h"},
		["Source Files"] = {"rlExtrasC/**.c"},
	}
	files {"rlExtrasC/**.h", "rlExtrasC/**.c"}
	
project "rlExtrasCPP"
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		links {"raylib"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "./rlExtrasCPP/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src","rlExtrasCPP"}
	vpaths 
	{
		["Header Files"] = { "rlExtrasCPP/**.h"},
		["Source Files"] = {"rlExtrasCPP/**.cpp"},
	}
	files {"rlExtrasCPP/**.h", "rlExtrasCPP/**.cpp"}

group "Examples C"
project "FPCameraCSample"
	kind "ConsoleApp"
	location "Examples/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "Examples/FPCameraCSample/**.h"},
		["Source Files"] = {"Examples/FPCameraCSample/**.c"},
	}
	files {"Examples/FPCameraCSample/**.c", "Examples/FPCameraCSample/**.h"}

	links {"raylib","rlExtrasC"}
	
	includedirs { "Examples/FPCameraCSample", "raylib/src", "FPCameraC/" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
		
project "TOprbitCameraCSample"
	kind "ConsoleApp"
	location "Examples/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "Examples/TOprbitCameraCSample/**.h"},
		["Source Files"] = {"Examples/TOprbitCameraCSample/**.c"},
	}
	files {"Examples/TOprbitCameraCSample/**.c", "Examples/TOprbitCameraCSample/**.h"}

	links {"raylib","rlExtrasC"}
	
	includedirs { "Examples/TOprbitCameraCSample", "raylib/src", "rlExtrasC/" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}

project "RLSpritesCSample"
	kind "ConsoleApp"
	location "Examples/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "Examples/RLSpritesCSample/**.h"},
		["Source Files"] = {"Examples/RLSpritesCSample/**.c"},
	}
	files {"Examples/RLSpritesCSample/**.c", "Examples/RLSpritesCSample/**.h"}

	links {"raylib","rlExtrasC"}
	
	includedirs { "Examples/RLSpritesCSample", "raylib/src", "rlExtrasC" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
		
		
group "Examples Cpp"
project "FPCameraCppSample"
	kind "ConsoleApp"
	location "Examples/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "Examples/FPCameraCppSample/**.h"},
		["Source Files"] = {"Examples/FPCameraCppSample/**.cpp"},
	}
	files {"Examples/FPCameraCppSample/**.cpp", "Examples/FPCameraCppSample/**.h"}

	links {"raylib","rlExtrasCPP"}
	
	includedirs { "Examples/FPCameraCppSample", "raylib/src", "rlExtrasCPP" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
		

project "RLSpritesCppSample"
	kind "ConsoleApp"
	location "Examples/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "Examples/RLSpritesCppSample/**.h"},
		["Source Files"] = {"Examples/RLSpritesCppSample/**.c"},
	}
	files {"Examples/RLSpritesCppSample/**.c", "Examples/RLSpritesCppSample/**.h"}

	links {"raylib","rlExtrasCPP"}
	
	includedirs { "Examples/RLSpritesCppSample", "raylib/src", "rlExtrasCPP" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}