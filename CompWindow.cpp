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
#include <stdlib.h>
#include "InputContext.h"
#include "HelperFunctions.h"
#include "Globals.h"
#include "CompString.h"
#include "CompWindow.h"

namespace /* unnamed */
{
    const int caretWidth = 2;

    BOOL ClientToScreenXY(HWND wnd, LONG* x, LONG* y) {
        POINT temp;
        temp.x = *x;
        temp.y = *y;
        BOOL retValue = ClientToScreen(wnd, &temp);
        *x = temp.x;
        *y = temp.y;
        return retValue;
    }

} // unnamed namespace


CompWindow::CompWindow()
{
    // Assign the Windows' default message font to hFont_
    NONCLIENTMETRICS ncMetrics;
    ncMetrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncMetrics, 0);
    hFont_ = CreateFontIndirect(&ncMetrics.lfMessageFont);
}

CompWindow::~CompWindow()
{
    if (hWnd_)
        DestroyWindow(hWnd_);
    if (hFont_)
        DeleteObject(hFont_);
}

void CompWindow::create(HWND uiWnd)
{
    if (!hWnd_) {
        hWnd_ = CreateWindowEx(0,
                               compClassName,
                               NULL,
                               WS_POPUP | WS_DISABLED,
                               0, 0, 1, 1,
                               uiWnd,
                               NULL,
                               moduleInstance,
                               (LPVOID)this
                               );
    }
    ShowWindow(hWnd_, SW_HIDE);
}

void CompWindow::show()
{
    ShowWindow(hWnd_, SW_SHOWNOACTIVATE);
}

void CompWindow::hide()
{
    ShowWindow(hWnd_, SW_HIDE);
}

void CompWindow::paint()
{
    // Return if there's no region to update
    RECT rect;
    if (!GetUpdateRect(hWnd_, &rect, FALSE))
        return;

    PAINTSTRUCT ps;

    HDC dc = BeginPaint(hWnd_, &ps);

    InputContext imc(getImc(hWnd_));
    CompString cs(imc);

    if (imc.lock() && cs.lock() && cs.compStr.size() != 0) {
        HFONT oldFont;
        if (hFont_)
            oldFont = (HFONT)SelectObject(dc, hFont_);

        RECT clientRect;
        GetClientRect(hWnd_, &clientRect);

        // Draw the composition text
        UINT oldAlign = SetTextAlign(dc, TA_LEFT | TA_TOP);
        ExtTextOut(dc, 0, 0, ETO_OPAQUE, &clientRect, cs.compStr.ptr(), cs.compStr.size(), NULL);
        SetTextAlign(dc, oldAlign);

        // Draw the underline
        if (size_t otmSize = GetOutlineTextMetrics(dc, 0, NULL)) {
            if (OUTLINETEXTMETRIC* otm = static_cast<OUTLINETEXTMETRIC*>(malloc(otmSize))) {
                if (GetOutlineTextMetrics(dc, otmSize, otm) != 0) {
                    int thickness = otm->otmsUnderscoreSize;
                    int posY = otm->otmTextMetrics.tmAscent - otm->otmsUnderscorePosition + thickness / 2;

                    LOGBRUSH brush;
                    brush.lbStyle = BS_SOLID;
                    brush.lbColor = RGB(0, 0, 0);
                    HPEN hPen = ExtCreatePen(PS_GEOMETRIC | PS_DASH | PS_ENDCAP_FLAT,
                                             thickness, &brush, 0, NULL);
                    HPEN oldPen = static_cast<HPEN>(SelectObject(dc, hPen));
                    MoveToEx(dc, 0, posY, NULL);
                    LineTo(dc, clientRect.right - caretWidth, posY);
                    SelectObject(dc, oldPen);
                }

                free(otm);
            }
        }

        // Draw the caret
        {
            int caretPos[maxCompLen];

            GCP_RESULTS gcpResult = {};
            gcpResult.lStructSize = sizeof(GCP_RESULTS);
            gcpResult.lpCaretPos = caretPos;
            gcpResult.nGlyphs = cs.compStr.size();
            gcpResult.nMaxFit = cs.compStr.size();

            GetCharacterPlacement(dc, cs.compStr.ptr(), cs.compStr.size(), 0, &gcpResult, 0);

            RECT caret;
            caret.top = clientRect.top;
            caret.bottom = clientRect.bottom;
            if (cs.cursorPos() < cs.compStr.size()) {
                caret.left = clientRect.left + caretPos[cs.cursorPos()];
            } else {
                caret.left = clientRect.right - caretWidth;
            }
            caret.right = caret.left + caretWidth;
            InvertRect(dc, &caret);
        }

        if (hFont_)
            SelectObject(dc, oldFont);
    }

    EndPaint(hWnd_, &ps);
}

void CompWindow::update()
{
    InputContext imc(getImc(hWnd_));
    CompString cs(imc);

    if (imc.lock() && cs.lock() && cs.compStr.size() != 0) {
        RECT area;
        if (imc->cfCompForm.dwStyle & CFS_RECT) {
            area = imc->cfCompForm.rcArea;
            ClientToScreenXY(imc->hWnd, &area.left, &area.top);
            ClientToScreenXY(imc->hWnd, &area.right, &area.bottom);
        } else {
            area = monitorWorkareaFromWindow(imc->hWnd);
        }

        POINT pos = imc->cfCompForm.ptCurrentPos;
        ClientToScreen(imc->hWnd, &pos);

        SIZE size;
        
        // Compute the window size.
        HDC dc = GetDC(hWnd_);
        HFONT oldFont;
        if (hFont_)
            oldFont = static_cast<HFONT>(SelectObject(dc, hFont_));
        GetTextExtentPoint32(dc, cs.compStr.ptr(), cs.compStr.size(), &size);
        if (hFont_)
            SelectObject(dc, oldFont);
        ReleaseDC(hWnd_, dc);

        size.cx += caretWidth; // space for the cursor

        GetClientRect(imc->hWnd, &area);

        SetWindowPos(hWnd_, NULL, pos.x, pos.y, size.cx, size.cy,
                     SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS);
        InvalidateRect(hWnd_, NULL, FALSE);
    }
}

void CompWindow::setFont(LOGFONT* logFont)
{
    if (HFONT newFont = CreateFontIndirect(logFont)) {
        if (hFont_)
            DeleteObject(hFont_);
        hFont_ = newFont;
    }
}

LRESULT CALLBACK CompWindow::windowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        {
            CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(wnd, 0, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        }
        return 0;
    case WM_PAINT:
        if (CompWindow* compWnd = reinterpret_cast<CompWindow*>(GetWindowLongPtr(wnd, 0)))
            compWnd->paint();
        return 0;
    default:
        return DefWindowProc(wnd, msg, wParam, lParam);
    }
}

void CompWindow::registerClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_IME | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = windowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = sizeof(LONG_PTR);
    wcex.hInstance      = moduleInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = compClassName;
    wcex.hIconSm        = NULL;

    RegisterClassEx(&wcex);
}

void CompWindow::unregisterClass()
{
    UnregisterClass(compClassName, moduleInstance);
}
