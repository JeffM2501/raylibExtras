function examples ()
	group "Examples"
	project "core_basic_window"
		kind "ConsoleApp"
		location "examples/core/"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"
		cppdialect "C++17"
		
		includedirs { "src"}
		vpaths 
		{
			["Header Files"] = { "examples/core/**.h"},
			["Source Files"] = {"examples/core/**.c"},
		}
		files {"examples/core/core_basic_window.c"}

		links {"raylib"}
		
		includedirs { "src" }
		
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
			dependson {"raylib"}
			links {"winmm", "raylib.lib"}
			libdirs {"bin/%{cfg.buildcfg}"}
			
		filter "action:gmake*"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
			
	project "core_basic_window_cpp"
		kind "ConsoleApp"
		location "examples/core/"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"
		cppdialect "C++17"
		
		includedirs { "src"}
		vpaths 
		{
			["Header Files"] = { "examples/core/**.h"},
			["Source Files"] = {"examples/core/**.c"},
		}
		files {"examples/core/core_basic_window.cpp"}

		links {"raylib"}
		
		includedirs { "src" }
		
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
			dependson {"raylib"}
			links {"winmm", "raylib.lib"}
			libdirs {"bin/%{cfg.buildcfg}"}
			
		filter "action:gmake*"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
end