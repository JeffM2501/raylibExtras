function raylib ()
	
	project "raylib"
		filter "configurations:Debug.DLL OR Release.DLL"
			kind "SharedLib"
			defines {"BUILD_LIBTYPE_SHARED"}
			
		filter "configurations:Debug OR Release"
			kind "StaticLib"
			
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
			links {"winmm"}
			
		filter "action:gmake*"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
					
		filter{}
		
		location "./"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"
		cppdialect "C++17"
		
		includedirs { "../../raylib/src", "../../raylib/src/external/glfw/include"}
		vpaths 
		{
			["Header Files"] = { "../../raylib/src/**.h"},
			["Source Files/*"] = {"../../raylib/src/**.c"},
		}
		files {"../../raylib/src/*.h", "../../raylib/src/*.c"}
		
		defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
end