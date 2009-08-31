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
