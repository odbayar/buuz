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
#include <string.h>
#include "common.h"
#include "composition_engine.h"
#include "compose.h"


namespace /* anonymous */
{

    const LPARAM GCS_COMP_READ_ALL =
        GCS_COMPREADSTR | GCS_COMPREADATTR | GCS_COMPREADCLAUSE;

    const LPARAM GCS_COMP_ALL =
        GCS_COMPSTR | GCS_COMPATTR | GCS_COMPCLAUSE;

    const LPARAM GCS_RESULT_READ_ALL =
        GCS_RESULTREADSTR | GCS_RESULTREADCLAUSE;

    const LPARAM GCS_RESULT_ALL =
        GCS_RESULTSTR | GCS_RESULTCLAUSE;

} // anonymous namespace


void finishComp(InputContext* imc, CompString* cs)
{
    cs->resultReadStr.setData(cs->compReadStr.ptr(), cs->compReadStr.size());
    cs->resultStr.setData(cs->compStr.ptr(), cs->compStr.size());
    cs->updateResultReadClause();
    cs->updateResultClause();

    cs->clearComp();

    imc->generateMessage(WM_IME_COMPOSITION, 0, GCS_RESULT_READ_ALL | GCS_RESULT_ALL);
    imc->generateMessage(WM_IME_ENDCOMPOSITION, 0, 0);
}

void cancelComp(InputContext* imc, CompString* cs)
{
    cs->clearCompAndResult();

    imc->generateMessage(WM_IME_COMPOSITION, 0, 0);
    imc->generateMessage(WM_IME_ENDCOMPOSITION, 0, 0);
}

BOOL processKey(InputContext* imc, UINT virtKey, UINT scanCode, CONST BYTE* keyState)
{
    BOOL retValue = FALSE;

    if (imc->ptr()->fOpen)
    {
        CompString cs(imc);
        if (cs.lock())
        {
            WCHAR charCode;
            if (ToUnicode(virtKey, scanCode, keyState, &charCode, 1, 0) == 0)
                charCode = 0;

            if (cs.compStr.size() == 0)
            {
                if (compositionEngine->isInputChar(charCode))
                    retValue = TRUE;
            }
            else
            {
                if (compositionEngine->isInputChar(charCode))
                    retValue = TRUE;
                else
                {
                    switch (virtKey)
                    {
                    case VK_SHIFT:
                    case VK_CAPITAL:
                    case VK_LEFT:
                    case VK_RIGHT:
                    case VK_BACK:
                    case VK_DELETE:
                        retValue = TRUE;
                        break;
                    default:
                        finishComp(imc, &cs);
                    }
                }
            }
        }
    }

    return retValue;
}

void toAsciiEx(InputContext* imc, UINT virtKey, UINT scanCode,
               WCHAR charCode, CONST BYTE* keyState)
{
    CompString cs(imc);

    if (!cs.lock())
        return;

    bool madeChanges = false;
    bool uppercase = (((keyState[VK_SHIFT]   & 0x80u) != 0) !=
                      ((keyState[VK_CAPITAL] & 0x01u) != 0));

    if (compositionEngine->isInputChar(charCode))
    {
        if (cs.compStr.size() == 0)
        {
            cs.clearCompAndResult();
            imc->generateMessage(WM_IME_STARTCOMPOSITION, 0, 0);
        }

        if (cs.compReadStr.size() < maxCompLen - 1)
        {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            cs.compReadStr.insert(readCursorPos, charCode);
            cs.compReadAttr.insert(readCursorPos, ATTR_INPUT);

            compositionEngine->readToComp(&cs);

            cs.setCursorPos(cs.compStr.size() - backCursorPos);
            cs.setDeltaStart(0);

            madeChanges = true;
        }
        else
            MessageBeep((UINT)-1);
    }
    else if (virtKey == VK_BACK)
    {
        if (cs.compReadStr.size() != 0)
        {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            if (readCursorPos > 0)
            {
                cs.compReadStr.erase(readCursorPos - 1);
                cs.compReadAttr.erase(readCursorPos - 1);

                compositionEngine->readToComp(&cs);

                cs.setCursorPos(cs.compStr.size() - backCursorPos);
                cs.setDeltaStart(0);

                madeChanges = true;
            }
        }
    }
    else if (virtKey == VK_DELETE)
    {
        if (cs.compReadStr.size() != 0)
        {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            if (readCursorPos < cs.compReadStr.size())
            {
                cs.compReadStr.erase(readCursorPos);
                cs.compReadAttr.erase(readCursorPos);

                compositionEngine->readToComp(&cs);

                cs.setDeltaStart(0);

                madeChanges = true;
            }
        }
    }
    else if (virtKey == VK_LEFT)
    {
        if (cs.compStr.size() != 0)
        {
            if (cs.cursorPos() > 0)
            {
                compositionEngine->compToRead(&cs);

                cs.setDeltaStart(cs.cursorPos() - 1);
                cs.setCursorPos(cs.cursorPos() - 1);

                madeChanges = true;
            }
        }
    }
    else if (virtKey == VK_RIGHT)
    {
        if (cs.compStr.size() != 0)
        {
            if (cs.cursorPos() < cs.compStr.size())
            {
                compositionEngine->compToRead(&cs);

                cs.setDeltaStart(cs.cursorPos());
                cs.setCursorPos(cs.cursorPos() + 1);

                madeChanges = true;
            }
        }
    }

    if (cs.compStr.size() != 0)
    {
        if (madeChanges)
        {
            imc->generateMessage(
                    WM_IME_COMPOSITION,
                    0,
                    GCS_COMP_READ_ALL | GCS_COMP_ALL |
                      GCS_CURSORPOS | GCS_DELTASTART
                    );
        }
    }
    else
        cancelComp(imc, &cs);
}
