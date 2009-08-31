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
