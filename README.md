# Raylib Extras - RLAssets
A simple asset management system for raylib

## About
This is a simple asset management system that works with raylib. It lets a game define a set of root folders to use for assets and then will managed all the OS specific bookeeping of getting paths correct. It has functions to find the currently running application on any OS, regardless of how it's started or what the current working directory is set to. It will also merge multiple roots into one virtual relative file system, so a game can have a base set of assets with the game program but an option set of "mod" files in other paths that override root assets.

## Useage
Add the 3 files (two .cpp and one .h) to your game proejct.
RLAssets uses C++ but provides a pure C interface so it can be used by both C and C++ code.

include "RLAssets.h" in your game code to use the functions.

Always start by defining the inital asset root path, usualy relative to the application folder.

// define an asset root in the "resources" folder relative to the application
rlas_SetAssetRootPath("resources", true);

If you wish to add addtional asset folders that are outside your asset root path (such as mod folders, or folders from a user profile), simply add them by calling rlas_AddAssetResourcePath.

From there you can load files using just the relative path and the '/' seperator character, RLAssets will take care of translating the path for your OS.

// load textures from relative paths
Texture tx1 = LoadTexture(rlas_GetAssetPath("parrots.png"));
Texture tx2 = LoadTexture(rlas_GetAssetPath("tiles/stone.png"));

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