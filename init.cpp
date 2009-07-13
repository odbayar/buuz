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
#include "ui_window.h"
#include "comp_window.h"
#include "status_window.h"
#include "composition_engine.h"

HINSTANCE moduleInstance;

BOOL CALLBACK DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        logToFile("DllMain -> DLL_PROCESS_ATTACH");

        moduleInstance = instance;
        compositionEngine = new CompositionEngine;

        UiWindow::registerClass();
        StatusWindow::registerClass();
        CompWindow::registerClass();
        break;
    case DLL_PROCESS_DETACH:
        logToFile("DllMain -> DLL_PROCESS_DETACH");

        UiWindow::unregisterClass();
        StatusWindow::unregisterClass();
        CompWindow::unregisterClass();
        break;
    case DLL_THREAD_ATTACH:
        logToFile("DllMain -> DLL_THREAD_ATTACH");
        break;
    case DLL_THREAD_DETACH:
        logToFile("DllMain -> DLL_THREAD_DETACH");
        break;
    }
    return TRUE;
}
