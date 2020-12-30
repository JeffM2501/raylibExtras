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

#include "RLAssets.h"
#include "raylib.h"

#include <map>
#include <string>
#include <vector>
#include <algorithm>

#if defined(_WIN32)
constexpr char PathDelim = '\\';
#else
constexpr char PathDelim = '/';
#endif // OSs

typedef struct  
{
    std::string RelativeName;
    std::string PathOnDisk;
}rlas_AssetMeta;

typedef std::map<std::string, rlas_AssetMeta> MetaMap;

MetaMap AssetMap;

std::vector<std::string> AssetRootPaths;

std::string GetRelPath(const char* c)
{
    if (c == nullptr)
        return std::string();

    std::string upperPath = c;
    for (auto& c : upperPath)
        c = toupper(c);

    return upperPath;
}

void rlas_SetAssetRootPath(const char* path, bool relativeToApp)
{
    AssetRootPaths.clear();

    if (relativeToApp)
    {
        std::string appPath = rlas_GetApplicationBasePath();
        if (path != nullptr)
        {
            appPath += path;
            appPath += PathDelim;
        }

        rlas_AddAssetResourcePath(appPath.c_str());
    }
    else
    {
        rlas_AddAssetResourcePath(path);
    }
}

const char* rlas_GetAssetRootPath()
{
    if (AssetRootPaths.size() == 0)
        return nullptr;

    return AssetRootPaths[0].c_str();
}

void RecurseAddFiles(const std::string& root, const std::string& relRootPath)
{
    int count = 0;
    char** path = GetDirectoryFiles(root.c_str(), &count);

    std::vector<std::string> subDirs;
    for (int i = 0; i < count; ++i)
    {
        if (path[i] == nullptr || path[i][0] == '.')
            continue;

        std::string relPath = relRootPath + path[i];
        std::string fullPath = root + path[i];
        if (FileExists(fullPath.c_str()))
        {
            std::string upperPath = GetRelPath(relPath.c_str());

            rlas_AssetMeta meta;
            meta.RelativeName = relPath;
            meta.PathOnDisk = fullPath;

            AssetMap[upperPath] = meta;
        }
        else
        {
            subDirs.push_back(path[i]);
           
        }
    }
    ClearDirectoryFiles();

    for (auto subDir : subDirs)
    {
        RecurseAddFiles(root + subDir + PathDelim , relRootPath + subDir + "/");
    }
}

void rlas_AddAssetResourcePath(const char* path)
{
    if (path == nullptr)
        return;

    std::string root = path;

    AssetRootPaths.emplace_back(root);

    RecurseAddFiles(root, "");
}

const char* rlas_GetAssetPath(const char* path)
{
    MetaMap::iterator itr = AssetMap.find(GetRelPath(path));
    if (itr == AssetMap.end())
        return nullptr;

    return itr->second.PathOnDisk.c_str();
}

int rlas_AppendPath(const char* path, const char* subpath, char* destination, int lenght)
{
    std::string p = path;
    std::string sp = subpath;
    std::string result = p + PathDelim + sp;

    if (result.size() > lenght - 1)
        return -1;

#ifdef _WIN32
    strncpy_s(destination, lenght, result.c_str(), result.size());
#else
    strncpy(destination, result.c_str(), result.size());
#endif //_WIN32
    destination[result.size()] = '\0';

    return static_cast<int>(result.size());
}

int rlas_GetAssetsInPath(const char* path, bool recursive, char* results[])
{
    int count = 0;

    std::string upperPath = GetRelPath(path);

    for (auto& asset : AssetMap)
    {
        if (asset.first.rfind(upperPath) == 0)
        {
            bool isFile = asset.first.find_first_of('/', upperPath.length()) > asset.first.size();
            if (isFile || recursive)
            {
                if (results != nullptr)
                    results[count] = (char*)asset.second.RelativeName.c_str();
                ++count;
            }
        }
    }

    return count;
}
