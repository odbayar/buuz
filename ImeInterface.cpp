/*
 * Copyright 2009-2012 Odbayar Nyamtseren <odbayar.n@gmail.com>
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
#include <string.h>
#include "Globals.h"
#include "HelperFunctions.h"
#include "InputContext.h"
#include "CompString.h"
#include "Composer.h"

namespace /* unnamed */
{
    void initContext(InputContext& imc)
    {
        if (!(imc->fdwInit & INIT_CONVERSION)) {
            imc->fdwConversion = IME_CMODE_ALPHANUMERIC;
            imc->fdwInit |= INIT_CONVERSION;
        }

        if (!(imc->fdwInit & INIT_LOGFONT)) {
            NONCLIENTMETRICS ncMetrics;
            ncMetrics.cbSize = sizeof(NONCLIENTMETRICS);
            SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
                    sizeof(NONCLIENTMETRICS), &ncMetrics, 0);
            imc->lfFont.W = ncMetrics.lfMessageFont;
            imc->fdwInit |= INIT_LOGFONT;
        }

        if (!(imc->fdwInit & INIT_COMPFORM)) {
            imc->cfCompForm.dwStyle = CFS_POINT;
            imc->cfCompForm.ptCurrentPos.x = 0;
            imc->cfCompForm.ptCurrentPos.y = 0;
            if (IsWindow(imc->hWnd))
                GetClientRect(imc->hWnd, &imc->cfCompForm.rcArea);
            else
                memset(&imc->cfCompForm.rcArea, 0, sizeof(RECT));
            imc->fdwInit |= INIT_COMPFORM;
        }
    }

} // unnamed namespace

extern "C"
{
    BOOL WINAPI
    ImeInquire(LPIMEINFO imeInfo,
               LPTSTR uiWndClass,
               DWORD systemInfo)
    {
        if (!imeInfo)
            return FALSE;

        imeInfo->dwPrivateDataSize = sizeof(InputContext::PrivateData);
        imeInfo->fdwProperty = IME_PROP_UNICODE | IME_PROP_AT_CARET | IME_PROP_COMPLETE_ON_UNSELECT |
                               IME_PROP_KBD_CHAR_FIRST | IME_PROP_IGNORE_UPKEYS;
        imeInfo->fdwConversionCaps = IME_CMODE_ALPHANUMERIC;
        imeInfo->fdwSentenceCaps = 0;
        imeInfo->fdwUICaps = 0;
        imeInfo->fdwSCSCaps = SCS_CAP_COMPSTR | SCS_CAP_MAKEREAD;
        imeInfo->fdwSelectCaps = 0;

        wcscpy(uiWndClass, uiClassName);

        isWinLogonProcess = systemInfo & IME_SYSINFO_WINLOGON;

        return TRUE;
    }

    DWORD WINAPI
    ImeConversionList(HIMC hImc,
                      LPCTSTR src,
                      LPCANDIDATELIST dest,
                      DWORD bufLen,
                      UINT flag)
    {
        return 0; // We won't implement this API
    }

    BOOL WINAPI
    ImeConfigure(HKL kl, HWND wnd, DWORD mode, LPVOID data)
    {
        // TODO
        return FALSE;
    }

    BOOL WINAPI
    ImeDestroy(UINT reserved)
    {
        if (reserved != 0)
            return FALSE;
        return TRUE;
    }

    LRESULT WINAPI
    ImeEscape(HIMC hImc, UINT escape, LPVOID data)
    {
        switch (escape) {
        case IME_ESC_QUERY_SUPPORT:
            if (data) {
                switch (*(UINT*)data) {
                case IME_ESC_QUERY_SUPPORT:
                //case IME_ESC_GETHELPFILENAME:
                    return TRUE;
                }
            }
            return FALSE;
        case IME_ESC_GETHELPFILENAME:
            //{
            //    LPTSTR path = (LPTSTR)data;
            //    if (path && GetWindowsDirectory(path, MAX_PATH))
            //    {
            //        if (wcscat_s(path, MAX_PATH, "\\Help\\buuz.chm") == 0)
            //            return TRUE;
            //    }
            //}
            return FALSE;
        default:
            return FALSE;
        }
    }

    BOOL WINAPI
    ImeProcessKey(HIMC hImc,
                  UINT virtKey,
                  LPARAM lParam,
                  CONST LPBYTE keyState)
    {
        InputContext imc(hImc);

        if (!imc.lock())
            return FALSE;

        WCHAR charCode;
        if (ToUnicode(virtKey, HIWORD(lParam), keyState, &charCode, 1, 0) == 0)
            charCode = 0;
        return composer->processKey(imc, virtKey, charCode, keyState);
    }

    UINT WINAPI
    ImeToAsciiEx(UINT virtKey,
                 UINT scanCode,
                 CONST LPBYTE keyState,
                 LPTRANSMSGLIST transMsgList,
                 UINT state,
                 HIMC hImc)
    {
        InputContext imc(hImc);

        if (!imc.lock())
            return 0;

        imc.prv()->inImeToAsciiEx = true;
        imc.prv()->numMsgs = 0;
        imc.prv()->msgList = transMsgList;

        composer->toAsciiEx(imc, LOWORD(virtKey), HIWORD(virtKey), keyState);

        imc.prv()->inImeToAsciiEx = false;

        return imc.prv()->numMsgs;
    }

    BOOL WINAPI
    NotifyIME(HIMC hImc, DWORD action, DWORD index, DWORD value)
    {
        InputContext imc(hImc);
        CompString cs(imc);
        BOOL retValue = FALSE;

        if (!imc.lock())
            return retValue;

        switch (action) {
        case NI_CONTEXTUPDATED:
            switch (value) {
            case IMC_SETOPENSTATUS:
                {
                    if (!imc->fOpen && cs.lock() && cs.compStr.size() != 0)
                        composer->cancelComp(imc, cs);
                    retValue = TRUE;
                }
                break;
            case IMC_SETCONVERSIONMODE:
            case IMC_SETSENTENCEMODE:
            case IMC_SETCANDIDATEPOS:
            case IMC_SETCOMPOSITIONFONT:
            case IMC_SETCOMPOSITIONWINDOW:
                break;
            }
            break;

        case NI_COMPOSITIONSTR:
            if (cs.lock()) {
                switch (index) {
                case CPS_COMPLETE:
                    composer->finishComp(imc, cs);
                    retValue = TRUE;
                    break;
                case CPS_CONVERT:
                    break;
                case CPS_REVERT:
                    break;
                case CPS_CANCEL:
                    composer->cancelComp(imc, cs);
                    retValue = TRUE;
                    break;
                }
            }
            break;

        // We don't support candidate list.
        case NI_OPENCANDIDATE:
        case NI_CLOSECANDIDATE:
        case NI_SELECTCANDIDATESTR:
        case NI_CHANGECANDIDATELIST:
        case NI_SETCANDIDATE_PAGESTART:
        case NI_SETCANDIDATE_PAGESIZE:
            retValue = FALSE;
            break;
        }

        return retValue;
    }

    BOOL WINAPI
    ImeSelect(HIMC hImc, BOOL select)
    {
        InputContext imc(hImc);

        if (!imc.lock())
            return FALSE;

        if (select) {
            CompString cs(imc);
            if (!cs.create())
                return FALSE;

            ImmSetOpenStatus(hImc, TRUE);

            initContext(imc);
        } else {
            ImmSetOpenStatus(hImc, FALSE);
        }

        return TRUE;
    }

    BOOL WINAPI
    ImeSetActiveContext(HIMC hImc, BOOL activate)
    {
        InputContext imc(hImc);

        if (imc.lock()) {
            initContext(imc);
            return TRUE;
        } else {
            return FALSE;
        }
    }

    BOOL WINAPI
    ImeSetCompositionString(HIMC hImc,
                            DWORD index,
                            LPVOID comp,
                            DWORD compLen,
                            LPVOID read,
                            DWORD readLen)
    {
        // TODO
        return FALSE;
    }

    //
    // We won't support dictionary, so everything below this line is unimportant.
    //

    BOOL WINAPI
    ImeRegisterWord(LPCTSTR reading, DWORD style, LPCTSTR string)
    {
        return FALSE;
    }

    BOOL WINAPI
    ImeUnregisterWord(LPCTSTR reading, DWORD style, LPCTSTR string)
    {
        return FALSE;
    }

    UINT WINAPI
    ImeGetRegisterWordStyle(UINT item, LPSTYLEBUF styleBuf)
    {
        return FALSE;
    }

    UINT WINAPI
    ImeEnumRegisterWord(REGISTERWORDENUMPROC enumProc,
                        LPCTSTR reading,
                        DWORD style,
                        LPCTSTR string,
                        LPVOID data)
    {
        return FALSE;
    }

} // extern "C"
