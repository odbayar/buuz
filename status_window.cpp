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
#include "Globals.h"
#include "status_window.h"

StatusWindow::StatusWindow()
{
}

StatusWindow::~StatusWindow()
{
}

LRESULT CALLBACK StatusWindow::windowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        break;
    case WM_PAINT:
        break;
    default:
        return DefWindowProc(wnd, msg, wparam, lparam);
    }
    return 0;
}

void StatusWindow::registerClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_IME | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = windowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = sizeof(LONG_PTR) * 2;
    wcex.hInstance      = moduleInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = statusClassName;
    wcex.hIconSm        = NULL;

    RegisterClassEx(&wcex);
}

void StatusWindow::unregisterClass()
{
    UnregisterClass(statusClassName, moduleInstance);
}
