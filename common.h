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
