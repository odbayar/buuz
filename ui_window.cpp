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
#include <new>
#include "resource.h"
#include "common.h"
#include "input_context.h"
#include "comp_string.h"
#include "ui_window.h"

namespace /* anonymous */
{

    inline HIMC getImc(HWND hWnd)
    {
        return (HIMC)GetWindowLongPtr(hWnd, IMMGWLP_IMC);
    }

    inline UiWindow* getObject(HWND hWnd)
    {
        return (UiWindow*)GetWindowLongPtr(hWnd, IMMGWLP_PRIVATE);
    }

    inline void setObject(HWND hWnd, UiWindow* uiWnd)
    {
        SetWindowLongPtr(hWnd, IMMGWLP_PRIVATE, (LONG_PTR)uiWnd);
    }

} // anonymous namespace


UiWindow::UiWindow()
{
    softKbdWnd = NULL;
}

UiWindow::~UiWindow()
{
    if (softKbdWnd)
        ImmDestroySoftKeyboard(softKbdWnd);
}

LRESULT UiWindow::imeSetContext(HWND wnd, WPARAM activate, LPARAM iscBits)
{
    if (activate)
    {
        InputContext imc(getImc(wnd));

        if (imc.lock())
        {
            if (iscBits & ISC_SHOWUICOMPOSITIONWINDOW)
            {
                CompString cs(&imc);
                if (cs.lock() && cs.compStr.size() != 0)
                {
                    if (compWnd.create(wnd))
                    {
                        compWnd.update();
                        compWnd.show();
                    }
                }
            }
            if (iscBits & ISC_SHOWUIGUIDELINE)
                (void)0; // TODO;
        }
    }
    else
    {
        if (iscBits & ISC_SHOWUICOMPOSITIONWINDOW)
        {
            if (compWnd.isOn())
                compWnd.hide();
        }
        if (iscBits & ISC_SHOWUIGUIDELINE)
            (void)0; // TODO
    }

    return 0;
}

LRESULT UiWindow::imeControl(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    InputContext imc(getImc(wnd));

    switch (wParam)
    {
    case IMC_GETCANDIDATEPOS:
        if (imc.lock() && *(DWORD*)lParam < 4)
        {
            CANDIDATEFORM* candForm = (CANDIDATEFORM*)lParam;
            *candForm = imc.ptr()->cfCandForm[candForm->dwIndex];
            return 0;
        }
        return 1;

    case IMC_GETCOMPOSITIONWINDOW:
        if (imc.lock())
        {
            COMPOSITIONFORM* compForm = (COMPOSITIONFORM*)lParam;
            *compForm = imc.ptr()->cfCompForm;
            compForm->dwStyle = CFS_POINT | CFS_FORCE_POSITION;
            return 0;
        }
        return 1;

    case IMC_GETSTATUSWINDOWPOS:
        if (statusWnd.isOn())
        {
            RECT rect;
            if (statusWnd.getRect(&rect))
            {
                POINTS pos;
                pos.x = (SHORT)rect.left;
                pos.y = (SHORT)rect.top;
                return *(LRESULT*)&pos;
            }
        }
        return 0;

    default:
        return 1;
    }
}

void UiWindow::imeSelect(HWND wnd, WPARAM wParam, LPARAM lParam)
{
}

void UiWindow::imeNotify(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    InputContext imc(getImc(wnd));

    switch (wParam)
    {
    case IMN_CLOSESTATUSWINDOW:
        break;
    case IMN_OPENSTATUSWINDOW:
        break;
    case IMN_SETCONVERSIONMODE:
        break;
    case IMN_SETSENTENCEMODE:
        break;
    case IMN_SETOPENSTATUS:
        break;
    case IMN_SETCANDIDATEPOS:
        break;
    case IMN_SETCOMPOSITIONFONT:
        if (imc.lock())
        {
            compWnd.setFont((LOGFONT*)&imc.ptr()->lfFont);
            if (compWnd.isOn())
                compWnd.update();
        }
        break;
    case IMN_SETCOMPOSITIONWINDOW:
        if (compWnd.isOn())
            compWnd.update();
        break;
    case IMN_SETSTATUSWINDOWPOS:
        break;
    case IMN_GUIDELINE:
        break;
    case IMN_PRIVATE:
        break;
    case IMN_SOFTKBDDESTROYED:
        break;

    // We don't have a candidate window.
    case IMN_CHANGECANDIDATE:
    case IMN_OPENCANDIDATE:
    case IMN_CLOSECANDIDATE:
        break;
    }
}

void UiWindow::imeStartComposition(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (compWnd.create(wnd))
    {
        compWnd.update();
        compWnd.show();
    }
}

void UiWindow::imeComposition(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (compWnd.isOn())
        compWnd.update();
}

void UiWindow::imeEndComposition(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (compWnd.isOn())
        compWnd.hide();
}

LRESULT CALLBACK UiWindow::windowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    UiWindow* uiWnd;

    switch (msg)
    {
    case WM_IME_SETCONTEXT:
    case WM_IME_CONTROL:
    case WM_IME_SELECT:
    case WM_IME_NOTIFY:
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_IME_COMPOSITIONFULL:
        uiWnd = getObject(wnd);
        if (!uiWnd)
            return 0;
        break;
    }

    switch (msg)
    {
    case WM_CREATE:
        uiWnd = new(std::nothrow) UiWindow();
        if (!uiWnd)
            return -1;
        setObject(wnd, uiWnd);
        return 0;

    case WM_DESTROY:
        if ((uiWnd = getObject(wnd)) != NULL)
            delete uiWnd;
        return 0;

    case WM_IME_SETCONTEXT:
        return uiWnd->imeSetContext(wnd, wParam, lParam);

    case WM_IME_CONTROL:
        return uiWnd->imeControl(wnd, wParam, lParam);

    case WM_IME_SELECT:
        uiWnd->imeSelect(wnd, wParam, lParam);
        return 0;

    case WM_IME_NOTIFY:
        uiWnd->imeNotify(wnd, wParam, lParam);
        return 0;

    case WM_IME_STARTCOMPOSITION:
        uiWnd->imeStartComposition(wnd, wParam, lParam);
        return 0;

    case WM_IME_COMPOSITION:
        uiWnd->imeComposition(wnd, wParam, lParam);
        return 0;

    case WM_IME_ENDCOMPOSITION:
        uiWnd->imeEndComposition(wnd, wParam, lParam);
        return 0;

    case WM_IME_COMPOSITIONFULL:
        // How did this message get here?!
        return 0;

    default:
        return DefWindowProc(wnd, msg, wParam, lParam);
    }
}

void UiWindow::registerClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_IME;
    wcex.lpfnWndProc    = windowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = sizeof(LONG_PTR) * 2;
    wcex.hInstance      = moduleInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = uiClassName;
    wcex.hIconSm        = NULL;

    RegisterClassEx(&wcex);
}

void UiWindow::unregisterClass()
{
    UnregisterClass(uiClassName, moduleInstance);
}
