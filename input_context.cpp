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
#include "input_context.h"

InputContext::InputContext(HIMC hImc)
{
    handle_ = hImc;
    imc_ = NULL;
    prv_ = NULL;
}

InputContext::~InputContext()
{
    unlock();
}

bool InputContext::lock()
{
    if (!imc_ && handle_)
    {
        imc_ = ImmLockIMC(handle_);
        if (imc_)
        {
            prv_ = (ImcPrivate*)ImmLockIMCC(imc_->hPrivate);
            if (!prv_)
            {
                ImmUnlockIMC(handle_);
                imc_ = NULL;
            }
        }
    }
    return imc_ != NULL;
}

void InputContext::unlock()
{
    if (imc_)
    {
        ImmUnlockIMCC(imc_->hPrivate);
        prv_ = NULL;
        ImmUnlockIMC(handle_);
        imc_ = NULL;
    }
}

bool InputContext::attachMessage(TRANSMSG* msg, TRANSMSGLIST* msgList)
{
    HIMCC temp = ImmReSizeIMCC(imc_->hMsgBuf, sizeof(TRANSMSG) *
            (imc_->dwNumMsgBuf + (msgList ? msgList->uMsgCount + 1 : 1)));
    if (!temp)
        return false;
    imc_->hMsgBuf = temp;

    TRANSMSG* msgBuf = (TRANSMSG*)ImmLockIMCC(imc_->hMsgBuf);
    if (!msgBuf)
        return false;

    msgBuf += imc_->dwNumMsgBuf;

    if (msgList)
    {
        for (UINT i = 0; i < msgList->uMsgCount; ++i)
            *msgBuf++ = msgList->TransMsg[i];
        imc_->dwNumMsgBuf += msgList->uMsgCount;
    }

    *msgBuf = *msg;
    ++imc_->dwNumMsgBuf;

    ImmUnlockIMCC(imc_->hMsgBuf);

    return true;
}

bool InputContext::generateMessage(TRANSMSG* msg)
{
    bool retValue = false;

    if (prv_->msgList)
    {
        if (prv_->numMsgs < prv_->msgList->uMsgCount)
        {
            prv_->msgList->TransMsg[prv_->numMsgs] = *msg;
            ++prv_->numMsgs;
            retValue = true;
        }
        else if (prv_->numMsgs > prv_->msgList->uMsgCount)
        {
            if (attachMessage(msg))
            {
                ++prv_->numMsgs;
                retValue = true;
            }
        }
        else
        {
            if (attachMessage(msg, prv_->msgList))
            {
                ++prv_->numMsgs;
                retValue = true;
            }
        }
    }
    else
    {
        if (IsWindow(imc_->hWnd))
        {
            attachMessage(msg);
            ImmGenerateMessage(handle_);
            retValue = true;
        }
    }

    return retValue;
}

bool InputContext::generateMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    TRANSMSG msg;
    msg.message = message;
    msg.wParam = wParam;
    msg.lParam = lParam;
    return generateMessage(&msg);
}
