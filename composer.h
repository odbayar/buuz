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
