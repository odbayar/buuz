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

#ifndef COMPOSER_H
#define COMPOSER_H

#include "comp_string.h"

#include <set>
#include <hash_set>
#include "input_context.h"
#include "comp_string.h"

class Composer : private NonCopyable
{
public:
    Composer();
    virtual ~Composer() = 0;

    void finishComp(InputContext* imc, CompString* cs);
    void cancelComp(InputContext* imc, CompString* cs);

    BOOL processKey(InputContext* imc, UINT virtKey, UINT scanCode,
                    WCHAR charCode, CONST BYTE* keyState);
    void toAsciiEx(InputContext* imc, UINT virtKey, UINT scanCode,
                   WCHAR charCode, CONST BYTE* keyState);

protected:
    static const int maxRuleLen = 8;

    void addRule(const char* from, const char* to, DWORD flags);
    void exportConversionRules(const char* filename);
    void computeInputChars();
    bool isInputChar(WCHAR ch);

    virtual void compToRead(CompString* cs);
    virtual void readToComp(CompString* cs);

    // ConversionRule flags
    static const DWORD x_ac = 0x00000001u; // allow case conversion

    struct ConversionRule
    {
        WCHAR from[maxRuleLen];
        WCHAR to[maxRuleLen];
        DWORD flags;
    };

    struct HashCompare
    {
        enum
        {
            bucket_size = 1,
            min_buckets = 300
        };

        size_t operator()(const ConversionRule& rule) const
        {
            const WCHAR* str = rule.from;
            size_t ret = 0;
            while (*str)
            {
                ret = (ret << 4) + *str++;
                if (int tmp = (ret & 0xf0000000))
                {
                    ret = ret ^ (tmp >> 24);
                    ret = ret ^ tmp;
                }
            }
            return ret;
        }

        bool operator()(const ConversionRule& r1, const ConversionRule& r2) const
        {
            return wcscmp(r1.from, r2.from) < 0;
        }
    };

    typedef stdext::hash_multiset<ConversionRule, HashCompare> ConversionRuleSet;

    ConversionRuleSet rules_;
    std::set<WCHAR> inputChars_;
};

extern Composer* composer;

#endif // COMPOSER_H
