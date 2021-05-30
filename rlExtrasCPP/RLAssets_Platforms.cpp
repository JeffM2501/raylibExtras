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

#include <string>

#if defined(_WIN32)

#include <windows.h>
constexpr char PathDelim = '\\';

#elif defined(__linux__)
#include <unistd.h>
constexpr char PathDelim = '/';

#elif defined(__APPLE__)
#include <sys/syslimits.h>
#include <mach-o/dyld.h>
constexpr char PathDelim = '/';

#endif // OSs

std::string appDir;

const char* rlas_GetApplicationBasePath()
{
    if (appDir.size() == 0)
    {
        appDir = "/"; // default for everyone to start out with

#if defined(_WIN32)
        typedef DWORD(WINAPI* GetModuleFileNameFunc)(HANDLE, HMODULE, LPSTR, DWORD);
        GetModuleFileNameFunc getModuleFileNameExWPtr = nullptr;
        HMODULE lib = LoadLibrary(L"psapi.dll");
        if (lib == nullptr)
        {
            appDir = "\\";
        }
        else
        {
            getModuleFileNameExWPtr = (GetModuleFileNameFunc)GetProcAddress(lib, "GetModuleFileNameExA");

            if (getModuleFileNameExWPtr == nullptr)
            {
                appDir = "\\";
            }
            else
            {
                CHAR path[MAX_PATH];
                int len = getModuleFileNameExWPtr(GetCurrentProcess(), nullptr, path, MAX_PATH);
                if (len > 0)
                {
                    for (int i = len; i >= 0; --i)
                    {
                        if (path[i] == '\\')
                        {
                            path[i + 1] = '\0';
                            i = -1;
                        }
                    }
                    appDir = path;
                }
            }
            FreeLibrary(lib);
        }
#elif defined(__linux__)
        char path[4096 + 1];
        uint32_t size = sizeof(path);

        ssize_t len = readlink("/proc/self/exe", path, size);
        if (len > 0)
        {
            for (int i = len; i >= 0; --i)
            {
                if (path[i] == '/')
                {
                    path[i + 1] = '\0';
                    i = -1;
                }
            }
            appDir = path;
        }

#elif defined(__APPLE__)
        char path[PATH_MAX + 1];
        uint32_t size = sizeof(path);

        if (_NSGetExecutablePath(path, &size) == 0)
        {
            uint32_t len = strlen(path);
            for (uint32_t i = len; i >= 0; i--)
            {
                if (path[i] == '/' && path[i + 1] == '.')
                {
                    path[i + 1] = 0;
                    break
                }
            }
            appDir = path;
        }
#endif
    }
    return appDir.c_str();
}