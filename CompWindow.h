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

#ifndef COMP_WINDOW_H
#define COMP_WINDOW_H

#include "WindowBase.h"

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
