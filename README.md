# Raylib Extras - RLAssets
A simple asset management system for raylib with callback features

## About
This is a simple asset management system that works with raylib. It lets a game define a set of root folders to use for assets and then will managed all the OS specific bookeeping of getting paths correct. It has functions to find the currently running application on any OS, regardless of how it's started or what the current working directory is set to. It will also merge multiple roots into one virtual relative file system, so a game can have a base set of assets with the game program but an option set of "mod" files in other paths that override root assets.

### Zip File Support
The system supports using zip files as containers for the virtual file system. Any zip file found will be treated as a folder, and will be expanded into the virtual file system. This means you can use the contents of a zip file as if it was extracted to the disk.

## Useage
Add the 4 files (two .cpp and two .h) to your game proejct.
RLAssets uses C++ but provides a pure C interface so it can be used by both C and C++ code.

include "RLAssets.h" in your game code to use the functions.

Always start by defining calling rlas_Setup with the inital resource folder, usualy relative to the application folder.

rlas_Setup("resources", true);

If you wish to add addtional asset folders that are outside your asset root path (such as mod folders, or folders from a user profile), simply add them by calling rlas_AddAssetResourcePath.

From there you can load files using just the relative path and the '/' seperator character, using the normal raylib load functions. RLAssets will hook into the raylib file loading system and provide data as needed.

// load textures from relative paths
Texture tx1 = LoadTexture("parrots.png");
Image img1 = LoadImage("tiles/stone.png");

If you need to list the assets in a folder, you can  use rlas_GetAssetsInPath, it will return all the assets in the defined virtual path, from all resource directories.

// iterate assets in the virtual file system
int assetCount = rlas_GetAssetsInPath("/", true, nullptr);
char** buffer = (char**)malloc(assetCount * sizeof(char*));

rlas_GetAssetsInPath("/", true, buffer);

for (int i = 0; i < assetCount; ++i)
{
    PrintAsset (buffer[i]);
}
free(buffer);

When you are all done, call rlas_Cleanup() to clear out the virtual path and clean up any temportary files extracted from archives.