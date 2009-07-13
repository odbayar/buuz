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
#include "common.h"
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
