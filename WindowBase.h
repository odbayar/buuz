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

#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

#include "NonCopyable.h"

class WindowBase : private NonCopyable
{
public:
    WindowBase()
    {
        hWnd_ = NULL;
    }

    ~WindowBase()
    {
        if (hWnd_)
            DestroyWindow(hWnd_);
    }

    bool getRect(RECT* rect)
    {
        return GetWindowRect(hWnd_, rect) != FALSE;
    }

    bool isOn()
    {
        return hWnd_ != NULL;
    }

protected:

    static HIMC getImc(HWND wnd)
    {
        return (HIMC)GetWindowLongPtr(GetWindow(wnd, GW_OWNER), IMMGWLP_IMC);
    }

    HWND hWnd_;
};

#endif // WINDOW_BASE_H
