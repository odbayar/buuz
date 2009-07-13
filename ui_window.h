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

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "status_window.h"
#include "comp_window.h"

class UiWindow
{
public:
    HWND softKbdWnd;
    StatusWindow statusWnd;
    CompWindow compWnd;

public:
    UiWindow();
    ~UiWindow();

    static void registerClass();
    static void unregisterClass();

private:
    LRESULT imeSetContext(HWND, WPARAM, LPARAM);
    LRESULT imeControl(HWND, WPARAM, LPARAM);
    void imeSelect(HWND, WPARAM, LPARAM);
    void imeNotify(HWND, WPARAM, LPARAM);
    void imeStartComposition(HWND, WPARAM, LPARAM);
    void imeEndComposition(HWND, WPARAM, LPARAM);
    void imeComposition(HWND, WPARAM, LPARAM);
    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
};

#endif // UI_WINDOW_H
