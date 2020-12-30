# Raylib Extras - RLAssets
A simple asset management system for raylib

## About
This is a simple asset management system that works with raylib. It lets a game define a set of root folders to use for assets and then will managed all the OS specific bookeeping of getting paths correct. It has functions to find the currently running application on any OS, regardless of how it's started or what the current working directory is set to. It will also merge multiple roots into one virtual relative file system, so a game can have a base set of assets with the game program but an option set of "mod" files in other paths that override root assets.

## Useage
// define an asset root in the "resources" folder relative to the application
rlas_SetAssetRootPath("resources", true);

// load textures from relative paths
Texture tx1 = LoadTexture(rlas_GetAssetPath("/parrots.png"));
Texture tx2 = LoadTexture(rlas_GetAssetPath("/tiles/stone.png"));

// iterate assets in the virtual file system
int assetCount = rlas_GetAssetsInPath("/", true, nullptr);
char** buffer = (char**)malloc(assetCount * sizeof(char*));

rlas_GetAssetsInPath("/", true, buffer);

for (int i = 0; i < assetCount; ++i)
{
    PrintAsset (buffer[i]);
}
free(buffer);