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

#ifndef COMP_WINDOW_H
#define COMP_WINDOW_H

#include "window_base.h"

class CompWindow : public WindowBase
{
    HFONT hFont_;

public:
    CompWindow();
    ~CompWindow();

    bool create(HWND uiWnd);
    void show();
    void hide();
    void update();

    void setFont(LOGFONT* logFont);

    static void registerClass();
    static void unregisterClass();

private:
    void paint();

    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
};

#endif // COMP_WINDOW_H
