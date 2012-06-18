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
#include "InputContext.h"

InputContext::InputContext(HIMC hImc)
  : handle_(hImc),
    imc_(NULL),
    prv_(NULL)
{
}

InputContext::~InputContext()
{
    unlock();
}

bool InputContext::lock()
{
    if (!imc_ && handle_) {
        imc_ = ImmLockIMC(handle_);
        if (imc_) {
            prv_ = (PrivateData*)ImmLockIMCC(imc_->hPrivate);
            if (!prv_) {
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

bool InputContext::attachMessage(TRANSMSGLIST* msgList, const TRANSMSG& msg)
{
    // Resize the message buffer
    HIMCC temp = ImmReSizeIMCC(imc_->hMsgBuf, sizeof(TRANSMSG) *
            (imc_->dwNumMsgBuf + (msgList ? msgList->uMsgCount + 1 : 1)));
    if (!temp) {
        return false;
    }
    imc_->hMsgBuf = temp;

    // Copy msgList and msg into the message buffer
    TRANSMSG* msgBuf = (TRANSMSG*)ImmLockIMCC(imc_->hMsgBuf);
    if (!msgBuf) {
        return false;
    }
    msgBuf += imc_->dwNumMsgBuf;
    if (msgList) {
        for (UINT i = 0; i < msgList->uMsgCount; ++i)
            *msgBuf++ = msgList->TransMsg[i];
        imc_->dwNumMsgBuf += msgList->uMsgCount;
    }
    *msgBuf = msg;
    ++imc_->dwNumMsgBuf;
    ImmUnlockIMCC(imc_->hMsgBuf);

    return true;
}

bool InputContext::generateMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool retValue = false;

    TRANSMSG msg;
    msg.message = message;
    msg.wParam = wParam;
    msg.lParam = lParam;

    if (prv_->inImeToAsciiEx) {
        if (prv_->numMsgs < prv_->msgList->uMsgCount) {
            prv_->msgList->TransMsg[prv_->numMsgs] = msg;
            ++prv_->numMsgs;
            retValue = true;
        } else if (prv_->numMsgs > prv_->msgList->uMsgCount) {
            if (attachMessage(NULL, msg)) {
                ++prv_->numMsgs;
                retValue = true;
            }
        } else {
            if (attachMessage(prv_->msgList, msg)) {
                ++prv_->numMsgs;
                retValue = true;
            }
        }
    } else {
        if (IsWindow(imc_->hWnd)) {
            if (attachMessage(NULL, msg)) {
                ImmGenerateMessage(handle_);
                retValue = true;
            }
        }
    }

    return retValue;
}
