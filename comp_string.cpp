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
#include "InputContext.h"
#include "comp_string.h"

namespace /* unnamed */
{
    struct myCompStr
    {
        COMPOSITIONSTRING cs_;
        BYTE  compReadAttr[compBufferSize];
        DWORD compReadClause[clauseBufferSize];
        WCHAR compReadStr[compBufferSize];
        BYTE  compAttr[compBufferSize];
        DWORD compClause[clauseBufferSize];
        WCHAR compStr[compBufferSize];
        DWORD resultReadClause[clauseBufferSize];
        WCHAR resultReadStr[compBufferSize];
        DWORD resultClause[clauseBufferSize];
        WCHAR resultStr[compBufferSize];
    };

} // unnamed namespace

CompString::CompString(InputContext& imc)
  : imc_(imc),
    cs_(NULL)
{
}

CompString::~CompString()
{
    unlock();
}

bool CompString::lock()
{
    if (!cs_ && imc_->hCompStr) {
        cs_ = (COMPOSITIONSTRING*)ImmLockIMCC(imc_->hCompStr);
        updateBufferWrappers();
    }
    return cs_ != NULL;
}

void CompString::unlock()
{
    if (cs_) {
        ImmUnlockIMCC(imc_->hCompStr);
        cs_ = NULL;
    }
}

bool CompString::create()
{
    HIMCC& handle = imc_->hCompStr;

    if (!handle) {
        handle = ImmCreateIMCC(sizeof(myCompStr));
        if (!handle)
            return false;
    } else if (ImmGetIMCCSize(handle) < sizeof(myCompStr)) {
        HIMCC temp = ImmReSizeIMCC(handle, sizeof(myCompStr));
        if (!temp)
            return false;
        handle = temp;
    }

    cs_ = (COMPOSITIONSTRING*)ImmLockIMCC(handle);
    if (!cs_)
        return false;

    ZeroMemory(cs_, sizeof(myCompStr));

    cs_->dwSize = sizeof(myCompStr);

    cs_->dwCompReadAttrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compReadAttr;
    cs_->dwCompReadClauseOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compReadClause;
    cs_->dwCompReadStrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compReadStr;

    cs_->dwCompAttrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compAttr;
    cs_->dwCompClauseOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compClause;
    cs_->dwCompStrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->compStr;

    cs_->dwResultReadClauseOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->resultReadClause;
    cs_->dwResultReadStrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->resultReadStr;

    cs_->dwResultClauseOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->resultClause;
    cs_->dwResultStrOffset = (DWORD)(UINT_PTR)&((myCompStr*)0)->resultStr;

    ImmUnlockIMCC(handle);

    return true;
}

void CompString::clearComp()
{
    setCursorPos(0);
    setDeltaStart(0);

    compReadAttr.resize(0);
    compReadClause.resize(0);
    compReadStr.resize(0);
    compAttr.resize(0);
    compClause.resize(0);
    compStr.resize(0);
}

void CompString::clearResult()
{
    resultReadClause.resize(0);
    resultReadStr.resize(0);
    resultClause.resize(0);
    resultStr.resize(0);
}

void CompString::updateBufferWrappers()
{
    compReadAttr.init((BYTE*)((BYTE*)cs_ + cs_->dwCompReadAttrOffset), &cs_->dwCompReadAttrLen);
    compReadClause.init((DWORD*)((BYTE*)cs_ + cs_->dwCompReadClauseOffset), &cs_->dwCompReadClauseLen);
    compReadStr.init((WCHAR*)((BYTE*)cs_ + cs_->dwCompReadStrOffset), &cs_->dwCompReadStrLen);

    compAttr.init((BYTE*)((BYTE*)cs_ + cs_->dwCompAttrOffset), &cs_->dwCompAttrLen);
    compClause.init((DWORD*)((BYTE*)cs_ + cs_->dwCompClauseOffset), &cs_->dwCompClauseLen);
    compStr.init((WCHAR*)((BYTE*)cs_ + cs_->dwCompStrOffset), &cs_->dwCompStrLen);

    resultReadClause.init((DWORD*)((BYTE*)cs_ + cs_->dwResultReadClauseOffset), &cs_->dwResultReadClauseLen );
    resultReadStr.init((WCHAR*)((BYTE*)cs_ + cs_->dwResultReadStrOffset), &cs_->dwResultReadStrLen);

    resultClause.init((DWORD*)((BYTE*)cs_ + cs_->dwResultClauseOffset), &cs_->dwResultClauseLen);
    resultStr.init((WCHAR*)((BYTE*)cs_ + cs_->dwResultStrOffset), &cs_->dwResultStrLen);
}

void CompString::updateCompReadClause()
{
    compReadClause.resize(0);
    compReadClause.append(0);
    DWORD currentAttr = compReadAttr[0];
    for (DWORD pos = 1; pos < compReadAttr.size(); ++pos) {
        if (compReadAttr[pos] != currentAttr) {
            currentAttr = compReadAttr[pos];
            compReadClause.append(pos);
        }
    }
    compReadClause.append(compReadAttr.size());
}

void CompString::updateCompClause()
{
    compClause.resize(0);
    compClause.append(0);
    DWORD currentAttr = compAttr[0];
    for (DWORD pos = 1; pos < compAttr.size(); ++pos) {
        if (compAttr[pos] != currentAttr) {
            currentAttr = compAttr[pos];
            compClause.append(pos);
        }
    }
    compClause.append(compAttr.size());
}

void CompString::updateResultReadClause()
{
    resultReadClause.resize(0);
    resultReadClause.append(0);
    resultReadClause.append(resultReadStr.size());
}

void CompString::updateResultClause()
{
    resultClause.resize(0);
    resultClause.append(0);
    resultClause.append(resultStr.size());
}
