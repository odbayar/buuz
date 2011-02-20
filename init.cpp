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
#include "common.h"
#include "ui_window.h"
#include "comp_window.h"
#include "status_window.h"
#include "composer.h"

BOOL CALLBACK DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        {
            moduleInstance = instance;

            composer = new Composer;

            UiWindow::registerClass();
            StatusWindow::registerClass();
            CompWindow::registerClass();
        }
        break;
    case DLL_PROCESS_DETACH:
        {
            delete composer;
            UiWindow::unregisterClass();
            StatusWindow::unregisterClass();
            CompWindow::unregisterClass();
        }
        break;
    }
    return TRUE;
}
