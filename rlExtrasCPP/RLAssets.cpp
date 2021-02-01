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

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>
#include <memory>

#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
constexpr char PathDelim = '\\';
#include <fcntl.h> // For O_BINARY
#include <io.h> // For _setmode, _fileno, _isatty
#else
constexpr char PathDelim = '/';
#include <unistd.h> // For fileno, isatty
#endif // OSs

#include "zip_file.h"

typedef struct  
{
    std::string RelativeName;
    std::string PathOnDisk;
    std::shared_ptr<miniz_cpp::zip_file> ArchiveFile;
    miniz_cpp::zip_info ArchiveInfo;
}rlas_AssetMeta;

typedef std::map<std::string, rlas_AssetMeta> MetaMap;
typedef std::map<std::string, std::string> TempMap;

MetaMap AssetMap;
TempMap TempFiles;

std::vector<std::string> AssetRootPaths;

std::string AssetTempPath;

std::string ToUpper(const char* c)
{
    if (c == nullptr)
        return std::string();

    std::string upperPath = c;
    for (auto& c : upperPath)
        c = toupper(c);

    return upperPath;
}

void rlas_Cleanup()
{
    AssetRootPaths.clear();
    AssetMap.clear();

    for (auto& file : TempFiles)
    {
        try
        {
            remove(file.second.c_str());
        }
        catch (...)
        {
        	
        }
    }
        
    TempFiles.clear();
}

void rlas_SetTempPath(const char* path)
{
    if (path == nullptr)
        AssetTempPath.clear();
    else
        AssetTempPath = path;
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

void AddZipArchive(const char* archiveName, const std::string& archivePath, const std::string& relRootPath)
{
    std::string archiveRelPath = relRootPath;
    if (archiveName != nullptr)
        archiveRelPath += archiveName + std::string("/");

    std::shared_ptr<miniz_cpp::zip_file> archive = std::make_shared<miniz_cpp::zip_file>(archivePath.c_str());

    for (auto& info : archive->infolist())
    {
        if (info.file_size == 0)
            continue;

        std::string assetRelPath = archiveRelPath + info.filename;

        rlas_AssetMeta meta;
        meta.RelativeName = assetRelPath;
        meta.PathOnDisk = archivePath;
        meta.ArchiveFile = archive;
        meta.ArchiveInfo = info;

        std::string upperPath = ToUpper(assetRelPath.c_str());

        AssetMap[upperPath] = meta;
    }
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
            if (IsFileExtension(path[i], ".zip"))
            {
                AddZipArchive(GetFileNameWithoutExt(path[i]), fullPath, relRootPath);
            }
            else
            {
                std::string upperPath = ToUpper(relPath.c_str());

                rlas_AssetMeta meta;
                meta.RelativeName = relPath;
                meta.PathOnDisk = fullPath;
                meta.ArchiveFile = nullptr;

                AssetMap[upperPath] = meta;
            }
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

void rlas_AddAssetResourceArchive(const char* path, bool relativeToApp)
{
    std::string pathToUse = path;
    
    if (relativeToApp)
    {
        pathToUse = rlas_GetApplicationBasePath();
        if (path != nullptr)
        {
            pathToUse += path;
        }
    }
    AddZipArchive(nullptr, pathToUse, "");
}

const char* rlas_GetAssetPath(const char* path)
{
    std::string upperPath = ToUpper(path);
    MetaMap::iterator itr = AssetMap.find(upperPath);
    if (itr == AssetMap.end())
        return nullptr;

    if (itr->second.ArchiveFile != nullptr)
    {
        if (TempFiles.find(upperPath) == TempFiles.end())
        {
            if (AssetTempPath.empty())  // no place to extract, return null
                return nullptr;

            std::string tempName = itr->second.RelativeName;
            std::replace(tempName.begin(), tempName.end(), '/', '_');
            tempName = AssetTempPath + tempName;

            std::fstream stream(tempName, std::ios::binary | std::ios::out);
            stream << itr->second.ArchiveFile->open(itr->second.ArchiveInfo).rdbuf();

            TempFiles[upperPath] = tempName;
        }

        return TempFiles[upperPath].c_str();
    }

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

    std::string upperPath = ToUpper(path);

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

bool rlas_FileIsArchive(const char* path)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
        return false;

    return itr->second.ArchiveFile != nullptr;
}

Texture rlas_LoadTexture(const char* path)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
    {
        // can't be found, default texture
        Image img = GenImageChecked(256, 256, 64, 64, RAYWHITE, LIGHTGRAY);
        Texture texture = LoadTextureFromImage(img);
        UnloadImage(img);
        return texture;
    }
   
    if (itr->second.ArchiveFile != nullptr)
    {
        unsigned char* buffer = new unsigned char[itr->second.ArchiveInfo.file_size];
        itr->second.ArchiveFile->readBin(itr->second.ArchiveInfo, buffer);
        Image img = LoadImageFromMemory(GetExtension(itr->second.RelativeName.c_str()), buffer, (int)itr->second.ArchiveInfo.file_size);
        delete[](buffer);

        Texture tx = LoadTextureFromImage(img);
        UnloadImage(img);
        return tx;
    }

    return LoadTexture(itr->second.PathOnDisk.c_str());
}

Image rlas_LoadImage(const char* path)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
    {
        // can't be found, default image
        Image img = GenImageChecked(256, 256, 64, 64, RAYWHITE, LIGHTGRAY);
        return img;
    }

    if (itr->second.ArchiveFile != nullptr)
    {
        unsigned char* buffer = new unsigned char[itr->second.ArchiveInfo.file_size];
        itr->second.ArchiveFile->readBin(itr->second.ArchiveInfo, buffer);
        Image img = LoadImageFromMemory(GetExtension(itr->second.RelativeName.c_str()), buffer, (int)itr->second.ArchiveInfo.file_size);
        delete[](buffer);
        return img;
    }

    return LoadImage(itr->second.PathOnDisk.c_str());
}

unsigned int rlas_GetFileSize(const char* path)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
        return 0;

    if (itr->second.ArchiveFile != nullptr)
        return (unsigned int)itr->second.ArchiveInfo.file_size;

    unsigned int size = 0;

    if (!itr->second.PathOnDisk.empty())
    {
#ifdef _WIN32
        FILE* file = nullptr;
        fopen_s(&file, itr->second.PathOnDisk.c_str(), "rb");
#else
        FILE* file = fopen(itr->second.PathOnDisk.c_str(), "rb");
#endif //_WIN32

        if (file != nullptr)
        {
            // WARNING: On binary streams SEEK_END could not be found,
            // using fseek() and ftell() could not work in some (rare) cases
            fseek(file, 0, SEEK_END);
            size = ftell(file);
            fseek(file, 0, SEEK_SET);
            fclose(file);
        }
    }

    return size;
}

unsigned int rlas_GetFileText(const char* path, char* text)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
        return 0;

    if (itr->second.ArchiveFile != nullptr)
    {
        std::string data = itr->second.ArchiveFile->read(itr->second.ArchiveInfo);

        if (text != nullptr)
        {
#ifdef _WIN32
            strncpy_s(text, data.size(), data.c_str(), data.size());
#else
            strncpy(text, data.c_str(), data.size());
#endif //_WIN32
        }

        return (unsigned int)data.size();
    }

    char *data = LoadFileText(itr->second.PathOnDisk.c_str());
    unsigned int len = (unsigned int)strlen(data);
    
    if (text != nullptr)
    {
#ifdef _WIN32
        strncpy_s(text, len, data, len);
#else
        strncpy(text, data, len);
#endif //_WIN32
    }

    UnloadFileText((unsigned char*)data);

    return len;
}

unsigned int rlas_GetFileBytes(const char* path, void* data)
{
    MetaMap::iterator itr = AssetMap.find(ToUpper(path));
    if (itr == AssetMap.end())
        return 0;

    if (itr->second.ArchiveFile != nullptr)
    {
        return (unsigned int)itr->second.ArchiveFile->readBin(itr->second.ArchiveInfo, data);
    }

    unsigned int len = 0;
    unsigned char* buffer = LoadFileData(itr->second.PathOnDisk.c_str(), &len);

    if (data != nullptr)
    {
#ifdef _WIN32
        memcpy_s(data, len, buffer, len);
#else
        memcpy(data, buffer, len);
#endif //_WIN32
    }

    UnloadFileData(buffer);

    return len;
}   // Unload file text data allocated by LoadFileText()