/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLAssets * Simple Asset Managment System for Raylib
*
*   LICENSE: MIT
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#ifndef RLASSETS_H
#define RLASSETS_H

#include "raylib.h"

/// <summary>
/// Gets the application (exe) directory for the currently running program
/// </summary>
/// <returns>The path on disk in the current OSs format</returns>
const char* rlas_GetApplicationBasePath();

/// <summary>
/// adds a subpath (folder or file) to the specified path with the correct characters for the current OS
/// </summary>
/// <param name="path">the root path</param>
/// <param name="subpath">the sub path to append</param>
/// <param name="destination">the destination string</param>
/// <param name="lenght">the lenght of the destination string</param>
/// <returns>the lenght of the resulting combined path, -1 if the destination was not long enough</returns>
int rlas_AppendPath(const char* path, const char* subpath, char* destination, int lenght);

/// <summary>
/// Sets the initial asset path on disk
/// </summary>
/// <param name="path">The path to use as the asset root</param>
/// <param name="relativeToApp">When true the specified path will be used relative to the application root and should be in unix (/) format, when false the path specified is in the OSs format</param>
void rlas_SetAssetRootPath(const char* path, bool relativeToApp);

/// <summary>
/// Resets the virtual path system and cleans up any temporary files
/// </summary>
void rlas_Cleanup();

/// <summary>
/// Sets the temporary directory used when a file name is requested for assets in an archive
/// </summary>
/// <param name="path">The absolute path to use in OS format</param>
void rlas_SetTempPath(const char* path);

/// <summary>
/// Returns the top level asset root path
/// </summary>
/// <returns>The path on dis (OS format) of the inital asset root</returns>
const char* rlas_GetAssetRootPath();

/// <summary>
/// Adds an additional asset path to the search path for assets
/// The the specified path will be treated as '/' for relative paths
/// Any files that are duplicated in resource paths will be 'merged' into the virtual file structure and override older paths
/// </summary>
/// <param name="path">The resource path root to add</param>
void rlas_AddAssetResourcePath(const char* path);

/// <summary>
/// Adds zip file as if it was an extracted resource path
/// All files in the zip will be added to the root of the virtual path
/// Any files that are duplicated in resource paths will be 'merged' into the virtual file structure and override older paths
/// </summary>
/// <param name="path">The path of the archive to add</param>
/// <param name="relativeToApp">When true the specified path will be used relative to the application root and should be in unix (/) format, when false the path specified is in the OSs format</param>
void rlas_AddAssetResourceArchive(const char* path, bool relativeToApp);

/// <summary>
/// Gets the path on disk for an assets relative path
/// If multiple resource paths exist with the asset, the one added last will be returned.
/// If the asset is in an archive, and a temp directory is set, the asset will be extracted to the temp folder and that path will be returned
/// Temp files are deleted during cleanup
/// </summary>
/// <param name="path">The relative path of the asset to look up</param>
/// <returns>The path on disk of the asset</returns>
const char* rlas_GetAssetPath(const char* path);

/// <summary>
/// Returns a list of all relative asset names in a resource path
/// Call once with results as NULL to get the count to allocate a result buffer large enough
/// Then call again with buffer to get results.
/// </summary>
/// <param name="path">The relative path to search </param>
/// <param name="includeSubDirectories">Search into subdirectories</param>
/// <param name="results">A pointer to a character array to store the results, when null not used.</param>
/// <returns>The number of asset items found</returns>
int rlas_GetAssetsInPath(const char* path, bool includeSubDirectories, char** results);

/// <summary>
/// Returns true if the asset is part of an archive (zip) file
/// </summary>
/// <param name="path">The relative virtual path to the asset</param>
/// <returns>True if the asset is contained in an archive.</returns>
bool rlas_FileIsArchive(const char* path);

/// <summary>
/// Gets the file size of an asset from any source
/// </summary>
/// <param name="path">The relative virtual path to the asset</param>
/// <returns>The file size in bytes</returns>
unsigned int rlas_GetFileSize(const char* path);

#endif //RLASSETS_H

