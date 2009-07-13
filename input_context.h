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

#ifndef INPUT_CONTEXT_H
#define INPUT_CONTEXT_H

struct ImcPrivate
{
    UINT numMsgs;
    TRANSMSGLIST* msgList;
};

class InputContext : private NonCopyable
{
    HIMC handle_;
    INPUTCONTEXT* imc_;
    ImcPrivate* prv_;

public:
    InputContext(HIMC hImc);
    ~InputContext();

    bool lock();
    void unlock();

    bool generateMessage(TRANSMSG* msg);
    bool generateMessage(UINT message, WPARAM wParam, LPARAM lParam);

    HWND window() { return imc_->hWnd; }
    HIMC handle() { return handle_; }

    INPUTCONTEXT* ptr() { return imc_; }
    ImcPrivate* prv() { return prv_; }

private:
    bool attachMessage(TRANSMSG* msg, TRANSMSGLIST* msgList = NULL);
};

#endif // INPUT_CONTEXT_H
