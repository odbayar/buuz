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
#include <tchar.h>
#include "common.h"
#include "ui_window.h"
#include "comp_window.h"
#include "status_window.h"
#include "composer.h"
#include "composer_mongolian.h"
#include "composer_russian.h"

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
            else if (_tcsicmp(name, _T("buuz_rus.ime")) == 0)
            {
                _tcscpy(uiClassName, _T("BuuzRus"));
                _tcscpy(statusClassName, _T("BuuzRusStatus"));
                _tcscpy(compClassName, _T("BuuzRusComp"));
                composer = new ComposerRussian;
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
