/*
 * Copyright 2009 Odbayar Nyamtseren <odbayar.n@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <windows.h>
#include <immdev.h>
#include <string.h>
#include <stdio.h>
#include "HelperFunctions.h"

RECT monitorWorkareaFromPoint(POINT point)
{
    if (HMONITOR monitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST))
    {
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);
        if (GetMonitorInfo(monitor, &monitorInfo))
            return monitorInfo.rcWork;
    }

    RECT primaryMonitorWorkarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryMonitorWorkarea, 0);
    return primaryMonitorWorkarea;
}

RECT monitorWorkareaFromRect(LPRECT rect)
{
    if (HMONITOR monitor = MonitorFromRect(rect, MONITOR_DEFAULTTONEAREST))
    {
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);
        if (GetMonitorInfo(monitor, &monitorInfo))
            return monitorInfo.rcWork;
    }

    RECT primaryMonitorWorkarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryMonitorWorkarea, 0);
    return primaryMonitorWorkarea;
}

RECT monitorWorkareaFromWindow(HWND wnd)
{
    if (HMONITOR monitor = MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST))
    {
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);
        if (GetMonitorInfo(monitor, &monitorInfo))
            return monitorInfo.rcWork;
    }

    RECT primaryMonitorWorkarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryMonitorWorkarea, 0);
    return primaryMonitorWorkarea;
}

void logToFile(const char* format, ...)
{
#ifdef DEBUG

    HANDLE hFile;

    hFile = CreateFile(TEXT("c:\\buuz.txt"),
                       GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    char buf[4096];

    SYSTEMTIME now;
    GetLocalTime(&now);
    int len = sprintf_s(buf, sizeof(buf), "[%02d:%02d:%02d.%03d] ",
            now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);

    va_list args;
    va_start(args, format);
    len += vsprintf_s(buf + len, sizeof(buf) - len, format, args);

    if (len <= sizeof(buf) - 2)
    {
        buf[len] = '\n';
        buf[len + 1] = '\0';
    }

    OVERLAPPED eofWrite;
    memset(&eofWrite, 0, sizeof(eofWrite));
    eofWrite.Offset = eofWrite.OffsetHigh = 0xFFFFFFFF;

    WriteFile(hFile, buf, strlen(buf), NULL, &eofWrite);

    CloseHandle(hFile);

#endif
}
