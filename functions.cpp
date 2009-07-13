/*
 * Copyright (c) 2009 Odbayar Nyamtseren <odbayar.n@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <windows.h>
#include <immdev.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "input_context.h"

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
