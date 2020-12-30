#include "RLAssets.h"

#include <string>

#if defined(_WIN32)

#include <windows.h>
constexpr char PathDelim = '\\';

#elif defined(__linux__)
#include <unistd.h>
constexpr char PathDelim = '/';

#elif defined(__APPLE__)
#include <sys/syslimits.h>
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
            int len = strlne(path);
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
#endif
    }
    return appDir.c_str();
}