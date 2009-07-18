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
#include "composer_mongolian.h"

ComposerMongolian::ComposerMongolian()
{
    addRule("А"   , "А" , x_m | x_f | x_mm | x_ac);
    addRule("АI"  , "АЙ", x_m | x_f | x_mm | x_ac);
    addRule("О"   , "О" , x_m | x_f | x_mm | x_ac);
    addRule("ОI"  , "ОЙ", x_m | x_f | x_mm | x_ac);
    addRule("У"   , "У" , x_m | x_f | x_mm | x_ac);
    addRule("УI"  , "УЙ", x_m | x_f | x_mm | x_ac);
    addRule("Э"   , "Э" , x_m | x_f | x_mf | x_ac);
    addRule("ЭI"  , "ЭЙ", x_m | x_f | x_mf | x_ac);
    addRule("Ө"   , "Ө" , x_m | x_f | x_mf | x_ac);
    addRule("ӨI"  , "ӨЙ", x_m | x_f | x_mf | x_ac);
    addRule("Ү"   , "Ү" , x_m | x_f | x_mf | x_ac);
    addRule("ҮI"  , "ҮЙ", x_m | x_f | x_mf | x_ac);

    addRule("ИI"  , "ИЙ", x_m | x_f |    0 | x_ac);

    addRule("A"   , "А" , x_m | x_f | x_mm | x_ac);
    addRule("AI"  , "АЙ", x_m | x_f | x_mm | x_ac);
    addRule("B"   , "Б" , x_m | x_f |    0 | x_ac);
    addRule("C"   , "Ц" , x_m | x_f |    0 | x_ac);
    addRule("CH"  , "Ч" , x_m | x_f |    0 | x_ac);
    addRule("D"   , "Д" , x_m | x_f |    0 | x_ac);
    addRule("E"   , "Э" , x_m | x_f | x_mf | x_ac);
    addRule("EI"  , "ЭЙ", x_m | x_f | x_mf | x_ac);
    addRule("F"   , "Ф" , x_m | x_f |    0 | x_ac);
    addRule("G"   , "Г" , x_m | x_f |    0 | x_ac);
    addRule("H"   , "Х" , x_m | x_f |    0 | x_ac);
    addRule("I"   , "И" , x_m | x_f |    0 | x_ac);
    addRule("II"  , "ИЙ", x_m | x_f |    0 | x_ac);
    addRule("III" , "Ы" , x_m | x_f |    0 | x_ac);
    addRule("J"   , "Ж" , x_m | x_f |    0 | x_ac);
    addRule("K"   , "К" , x_m | x_f |    0 | x_ac);
    addRule("KH"  , "Х" , x_m | x_f |    0 | x_ac);
    addRule("L"   , "Л" , x_m | x_f |    0 | x_ac);
    addRule("M"   , "М" , x_m | x_f |    0 | x_ac);
    addRule("N"   , "Н" , x_m | x_f |    0 | x_ac);

    addRule("O"   , "О" , x_m |   0 |    0 | x_ac);
    addRule("OI"  , "ОЙ", x_m |   0 |    0 | x_ac);
    addRule("O\"" , "О" , x_m | x_f | x_mm | x_ac);
    addRule("O\"I", "ОЙ", x_m | x_f | x_mm | x_ac);
    addRule("\"O" , "О" , x_m | x_f | x_mm | x_ac);
    addRule("\"OI", "ОЙ", x_m | x_f | x_mm | x_ac);

    addRule("O"   , "Ө" ,   0 | x_f |    0 | x_ac);
    addRule("OI"  , "ӨЙ",   0 | x_f |    0 | x_ac);
    addRule("Q"   , "Ө" , x_m | x_f | x_mf | x_ac);
    addRule("QI"  , "ӨЙ", x_m | x_f | x_mf | x_ac);
    addRule("O'"  , "Ө" , x_m | x_f | x_mf | x_ac);
    addRule("O'I" , "ӨЙ", x_m | x_f | x_mf | x_ac);
    addRule("'O"  , "Ө" , x_m | x_f | x_mf | x_ac);
    addRule("'OI" , "ӨЙ", x_m | x_f | x_mf | x_ac);

    addRule("P"   , "П" , x_m | x_f |    0 | x_ac);
    addRule("R"   , "Р" , x_m | x_f |    0 | x_ac);
    addRule("S"   , "С" , x_m | x_f |    0 | x_ac);
    addRule("SH"  , "Ш" , x_m | x_f |    0 | x_ac);
    addRule("SXC" , "Щ" , x_m | x_f |    0 | x_ac);
    addRule("T"   , "Т" , x_m | x_f |    0 | x_ac);

    addRule("U"   , "У" , x_m |   0 |    0 | x_ac);
    addRule("UI"  , "УЙ", x_m |   0 |    0 | x_ac);
    addRule("U\"" , "У" , x_m | x_f | x_mm | x_ac);
    addRule("U\"I", "УЙ", x_m | x_f | x_mm | x_ac);
    addRule("\"U" , "У" , x_m | x_f | x_mm | x_ac);
    addRule("\"UI", "УЙ", x_m | x_f | x_mm | x_ac);

    addRule("U"   , "Ү" ,   0 | x_f |    0 | x_ac);
    addRule("UI"  , "ҮЙ",   0 | x_f |    0 | x_ac);
    addRule("W"   , "Ү" , x_m | x_f | x_mf | x_ac);
    addRule("WI"  , "ҮЙ", x_m | x_f | x_mf | x_ac);
    addRule("U'"  , "Ү" , x_m | x_f | x_mf | x_ac);
    addRule("U'I" , "ҮЙ", x_m | x_f | x_mf | x_ac);
    addRule("'U"  , "Ү" , x_m | x_f | x_mf | x_ac);
    addRule("'UI" , "ҮЙ", x_m | x_f | x_mf | x_ac);

    addRule("V"   , "В" , x_m | x_f |    0 | x_ac);
    addRule("X"   , "Х" , x_m | x_f |    0 | x_ac);
    addRule("Y"   , "Ы" , x_m | x_f |    0 | x_ac);
    addRule("YA"  , "Я" , x_m | x_f | x_mm | x_ac);
    addRule("YE"  , "Е" , x_m | x_f | x_mf | x_ac);
    addRule("YO"  , "Ё" , x_m | x_f | x_mm | x_ac);
    addRule("YU"  , "Ю" , x_m | x_f | x_mm | x_ac);
    addRule("Z"   , "З" , x_m | x_f |    0 | x_ac);

    addRule("\""  , "ъ" , x_m | x_f |    0 |    0);
    addRule("\"\"", "Ъ" , x_m | x_f |    0 |    0);
    addRule("'"   , "ь" , x_m | x_f |    0 |    0);
    addRule("''"  , "Ь" , x_m | x_f |    0 |    0);

    computeInputChars();

#ifdef DEBUG
    exportConversionRules("C:\\buuz_rules_mon.txt");
#endif
}

ComposerMongolian::~ComposerMongolian()
{
}

void ComposerMongolian::readToComp(CompString* cs)
{
    DWORD wordFlags = x_m;

    cs->compStr.resize(0);

    const WCHAR* ptr = cs->compReadStr.ptr();
    const WCHAR* end = ptr + cs->compReadStr.size();

    ConversionRule tempRule;

    while (ptr < end)
    {
        int fromLen = (int)min(3, end - ptr);
        memcpy(tempRule.from, ptr, fromLen * sizeof(WCHAR));

        for (; fromLen > 0; --fromLen)
        {
            tempRule.from[fromLen] = '\0';

            std::pair<ConversionRuleSet::const_iterator,
                      ConversionRuleSet::const_iterator>
                range = rules_.equal_range(tempRule);

            while (range.first != range.second)
            {
                const ConversionRule* rule = &*range.first;

                if ((wordFlags & rule->flags) == wordFlags)
                {
                    if (rule->flags & x_mf)
                        wordFlags = x_f;
                    else
                        if (rule->flags & x_mm)
                            wordFlags = x_m;

                    cs->compStr.append(rule->to, wcslen(rule->to));
                    ptr += wcslen(rule->from);

                    goto resultFound;
                }

                ++range.first;
            }
        }

        cs->compStr.append(*ptr++);

    resultFound:;
    }

    cs->compAttr.repeat(ATTR_INPUT, cs->compStr.size());
    cs->updateCompClause();
}
