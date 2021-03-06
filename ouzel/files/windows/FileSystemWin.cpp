// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include "FileSystemWin.hpp"
#include "utils/Log.hpp"

extern std::string DEVELOPER_NAME;
extern std::string APPLICATION_NAME;
static char TEMP_BUFFER[1024];

namespace ouzel
{
    FileSystemWin::FileSystemWin()
    {
        char* exePath = _pgmptr;

        if (exePath)
        {
            appPath = getDirectoryPart(exePath);
            Log(Log::Level::INFO) << "Application directory: " << appPath;
        }
        else
        {
            Log(Log::Level::ERR) << "Failed to get current directory";
        }
    }

    std::string FileSystemWin::getStorageDirectory(bool user) const
    {
        WCHAR szBuffer[MAX_PATH];
        HRESULT hr = SHGetFolderPathW(nullptr, (user ? CSIDL_LOCAL_APPDATA : CSIDL_COMMON_APPDATA) | CSIDL_FLAG_CREATE, nullptr, SHGFP_TYPE_CURRENT, szBuffer);
        if (FAILED(hr))
        {
            Log(Log::Level::ERR) << "Failed to get the path of the AppData directory, error: " << hr;
            return "";
        }

        WideCharToMultiByte(CP_UTF8, 0, szBuffer, -1, TEMP_BUFFER, sizeof(TEMP_BUFFER), nullptr, nullptr);
        std::string path = TEMP_BUFFER;

        path += DIRECTORY_SEPARATOR + DEVELOPER_NAME;

        if (!directoryExists(path))
        {
            if (MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, szBuffer, MAX_PATH) == 0)
            {
                Log(Log::Level::ERR) << "Failed to convert UTF-8 to wide char";
                return false;
            }

            if (!CreateDirectoryW(szBuffer, nullptr))
            {
                Log(Log::Level::ERR) << "Failed to create directory " << path;
                return "";
            }
        }

        path += DIRECTORY_SEPARATOR + APPLICATION_NAME;

        if (!directoryExists(path))
        {
            if (MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, szBuffer, MAX_PATH) == 0)
            {
                Log(Log::Level::ERR) << "Failed to convert UTF-8 to wide char";
                return false;
            }

            if (!CreateDirectoryW(szBuffer, nullptr))
            {
                Log(Log::Level::ERR) << "Failed to create directory " << path;
                return "";
            }
        }

        return path;
    }

    std::string FileSystemWin::getTempDirectory() const
    {
        WCHAR szBuffer[MAX_PATH];
        if (GetTempPathW(MAX_PATH, szBuffer))
        {
            if (WideCharToMultiByte(CP_UTF8, 0, szBuffer, -1, TEMP_BUFFER, sizeof(TEMP_BUFFER), nullptr, nullptr) == 0)
            {
                Log(Log::Level::ERR) << "Failed to convert UTF-8 to wide char";
                return "";
            }

            return TEMP_BUFFER;
        }

        return "";
    }

    bool FileSystemWin::isAbsolutePath(const std::string& path) const
    {
        WCHAR szBuffer[MAX_PATH];
        if (MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, szBuffer, MAX_PATH) == 0)
        {
            Log(Log::Level::ERR) << "Failed to convert UTF-8 to wide char";
            return false;
        }
        return PathIsRelativeW(szBuffer) == FALSE;
    }
}
