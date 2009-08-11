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

#ifndef COMMON_H
#define COMMON_H

// Derive from this class to make your class non-copyable.
class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private: /* yep, private! */
    NonCopyable(const NonCopyable&);
    const NonCopyable& operator=(const NonCopyable&);
};

extern HINSTANCE moduleInstance;
extern bool isWinLogonProcess;

extern TCHAR uiClassName[];
extern TCHAR statusClassName[];
extern TCHAR compClassName[];

void logToFile(const char* format, ...);

class PerformanceTimer
{
    const char* message_;
    LARGE_INTEGER start_;
    LARGE_INTEGER end_;
    LARGE_INTEGER freq_;
public:
    PerformanceTimer(const char* message)
    {
        message_ = message;
        QueryPerformanceFrequency(&freq_);
        QueryPerformanceCounter(&start_);
    }

    ~PerformanceTimer()
    {
        QueryPerformanceCounter(&end_);
        double deltaTime = (double)(end_.QuadPart - start_.QuadPart);
        logToFile("%s: %.6f", message_, deltaTime / freq_.QuadPart);
    }
};

RECT monitorWorkareaFromPoint(POINT point);
RECT monitorWorkareaFromRect(LPRECT rect);
RECT monitorWorkareaFromWindow(HWND wnd);

#endif // COMMON_H
