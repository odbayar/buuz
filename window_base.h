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

#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

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
