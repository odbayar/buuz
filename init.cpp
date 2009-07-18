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
#include <tchar.h>
#include "common.h"
#include "ui_window.h"
#include "comp_window.h"
#include "status_window.h"
#include "composer.h"
#include "composer_mongolian.h"

HINSTANCE moduleInstance;

BOOL CALLBACK DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        {
            moduleInstance = instance;

            TCHAR path[MAX_PATH];
            GetModuleFileName(instance, path, MAX_PATH);
            TCHAR* name = _tcsrchr(path, _T('\\')) + 1;

            if (_tcsicmp(name, _T("buuz_mon.ime")) == 0)
            {
                _tcscpy(uiClassName, _T("BuuzMon"));
                _tcscpy(statusClassName, _T("BuuzMonStatus"));
                _tcscpy(compClassName, _T("BuuzMonComp"));
                composer = new ComposerMongolian;
            }
            else
                return FALSE;

            UiWindow::registerClass();
            StatusWindow::registerClass();
            CompWindow::registerClass();
        }
        break;
    case DLL_PROCESS_DETACH:
        delete composer;
        UiWindow::unregisterClass();
        StatusWindow::unregisterClass();
        CompWindow::unregisterClass();
        break;
    }
    return TRUE;
}
