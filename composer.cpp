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
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "composer.h"

Composer* composer;

namespace /* unnamed */ {

    const LPARAM GCS_COMP_READ_ALL = GCS_COMPREADSTR | GCS_COMPREADATTR | GCS_COMPREADCLAUSE;
    const LPARAM GCS_COMP_ALL = GCS_COMPSTR | GCS_COMPATTR | GCS_COMPCLAUSE;
    const LPARAM GCS_RESULT_READ_ALL = GCS_RESULTREADSTR | GCS_RESULTREADCLAUSE;
    const LPARAM GCS_RESULT_ALL = GCS_RESULTSTR | GCS_RESULTCLAUSE;

    const BYTE cyrToUpper[] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
        0x50,0x01,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
        0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
        0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
        0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
        0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAE,
        0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
        0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
        0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
        0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE8,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
        0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
        };

    const BYTE cyrToLower[] = {
        0x00,0x51,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
        0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
        0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
        0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
        0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
        0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
        0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
        0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAF,0xAF,
        0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
        0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
        0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
        0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE9,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
        0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
        };

    inline WCHAR toUpper(WCHAR ch) {
        if (ch >= 'a' && ch <= 'z')
            return ch - ('a' - 'A');
        else
            if ((unsigned)ch >> 8 == 4)
                return cyrToUpper[ch & 0xFFu] + 0x0400;
            else
                return ch;
    }

    inline WCHAR toLower(WCHAR ch) {
        if (ch >= 'A' && ch <= 'Z')
            return ch + ('a' - 'A');
        else
            if ((unsigned)ch >> 8 == 4)
                return cyrToLower[ch & 0xFFu] + 0x0400;
            else
                return ch;
    }

    inline bool isUpper(WCHAR ch) {
        return (ch >= 'A' && ch <= 'Z') ||
            ((unsigned)ch >> 8 == 4 && cyrToLower[ch & 0xFFu] != (ch & 0xFFu));
    }

    inline bool isLower(WCHAR ch) {
        return (ch >= 'a' && ch <= 'z') ||
            ((unsigned)ch >> 8 == 4 && cyrToUpper[ch & 0xFFu] != (ch & 0xFFu));
    }

    void utf8ToUcs2(const char* src, WCHAR* dest, int destLen) {
        MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, destLen);
    }

} // unnamed namespace

Composer::Composer() {
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

Composer::~Composer() {
}

void Composer::addRule(const char* from, const char* to, DWORD flags) {
    WCHAR from_[maxRuleLen];
    WCHAR to_[maxRuleLen];
    utf8ToUcs2(from, from_, maxRuleLen);
    utf8ToUcs2(to, to_, maxRuleLen);
    int fromLen = wcslen(from_);
    int toLen = wcslen(to_);

    ConversionRule rule;
    rule.flags = flags;

    if (flags & x_ac) {
        bool alpha[10];
        int posMap[10];

        for (int i = 0, toPos = 0; i < fromLen; ++i) {
            alpha[i] = isUpper(from_[i]) || isLower(from_[i]);
            posMap[i] = alpha[i] && toPos < toLen ? toPos++ : -1;
        }

        BYTE state[10];
        int pos = 0;
        state[0] = 0;

        while (pos >= 0) {
            if (pos < fromLen) {
                switch (state[pos]) {
                case 0:
                case 1:
                    if (alpha[pos]) {
                        int toPos = posMap[pos];
                        if (state[pos] == 0) {
                            from_[pos] = toUpper(from_[pos]);
                            if (toPos >= 0)
                                to_[toPos] = toUpper(to_[toPos]);
                        } else {
                            from_[pos] = toLower(from_[pos]);
                            if (toPos >= 0)
                                to_[toPos] = toLower(to_[toPos]);
                        }
                        ++state[pos];
                    } else {
                        state[pos] = 2;
                    }
                    state[++pos] = 0;
                    break;
                case 2:
                    pos -= 1;
                    break;
                }
            } else {
                wcscpy(rule.from, from_);
                wcscpy(rule.to, to_);
                rules_.insert(rule);

                pos -= 1;
            }
        }
    } else {
        wcscpy(rule.from, from_);
        wcscpy(rule.to, to_);
        rules_.insert(rule);
    }
}

void Composer::exportConversionRules(const char* filename) {
    if (FILE* fp = fopen(filename, "w,ccs=UNICODE")) {
        for (ConversionRuleSet::const_iterator rule = rules_.begin();
                rule != rules_.end(); ++rule)
        {
            fwprintf(fp, L"%-3s = %-3s\n", rule->from, rule->to);
        }
        fclose(fp);
    }
}

void Composer::computeInputChars() {
    for (ConversionRuleSet::const_iterator rule = rules_.begin();
            rule != rules_.end(); ++rule)
    {
        for (const WCHAR* str = rule->from; *str; ++str)
            inputChars_.insert(*str);
    }
}

void Composer::readToComp(CompString* cs) {
    DWORD wordFlags = x_m;

    cs->compStr.resize(0);

    const WCHAR* ptr = cs->compReadStr.ptr();
    const WCHAR* end = ptr + cs->compReadStr.size();

    ConversionRule tempRule;

    while (ptr < end) {
        int fromLen = (int)min(3, end - ptr);
        memcpy(tempRule.from, ptr, fromLen * sizeof(WCHAR));

        for (; fromLen > 0; --fromLen) {
            tempRule.from[fromLen] = '\0';

            std::pair<ConversionRuleSet::const_iterator,
                      ConversionRuleSet::const_iterator>
                range = rules_.equal_range(tempRule);

            while (range.first != range.second) {
                const ConversionRule* rule = &*range.first;

                if ((wordFlags & rule->flags) == wordFlags) {
                    if (rule->flags & x_mf) {
                        wordFlags = x_f;
                    } else {
                        if (rule->flags & x_mm)
                            wordFlags = x_m;
                    }

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

void Composer::compToRead(CompString* cs) {
    cs->compReadStr.setData(cs->compStr.ptr(), cs->compStr.size());
    cs->compReadAttr.setData(cs->compAttr.ptr(), cs->compAttr.size());
    cs->updateCompReadClause();
}

bool Composer::isInputChar(WCHAR ch) {
    return inputChars_.find(ch) != inputChars_.end();
}

void Composer::finishComp(InputContext* imc, CompString* cs) {
    cs->resultReadStr.setData(cs->compReadStr.ptr(), cs->compReadStr.size());
    cs->resultStr.setData(cs->compStr.ptr(), cs->compStr.size());
    cs->updateResultReadClause();
    cs->updateResultClause();

    cs->clearComp();

    imc->generateMessage(WM_IME_COMPOSITION, 0, GCS_RESULT_READ_ALL | GCS_RESULT_ALL);
    imc->generateMessage(WM_IME_ENDCOMPOSITION, 0, 0);
}

void Composer::cancelComp(InputContext* imc, CompString* cs) {
    cs->clearCompAndResult();

    imc->generateMessage(WM_IME_COMPOSITION, 0, 0);
    imc->generateMessage(WM_IME_ENDCOMPOSITION, 0, 0);
}

BOOL Composer::processKey(InputContext* imc, UINT virtKey, UINT scanCode,
                          WCHAR charCode, CONST BYTE* keyState)
{
    BOOL retValue = FALSE;

    if (imc->ptr()->fOpen) {
        CompString cs(imc);
        if (cs.lock()) {
            if (cs.compStr.size() == 0) {
                if (isInputChar(charCode))
                    retValue = TRUE;
            } else {
                if (isInputChar(charCode)) {
                    retValue = TRUE;
                } else {
                    switch (virtKey) {
                    case VK_BACK:
                    case VK_DELETE:
                    case VK_SHIFT:
                    case VK_CAPITAL:
                    case VK_LEFT:
                    case VK_RIGHT:
                    case VK_HOME:
                    case VK_END:
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

void Composer::toAsciiEx(InputContext* imc, UINT virtKey, UINT scanCode,
                         WCHAR charCode, CONST BYTE* keyState)
{
    CompString cs(imc);

    if (!cs.lock())
        return;

    bool madeChanges = false;
    bool uppercase = (((keyState[VK_SHIFT]   & 0x80u) != 0) !=
                      ((keyState[VK_CAPITAL] & 0x01u) != 0));

    if (isInputChar(charCode)) {
        if (cs.compStr.size() == 0) {
            cs.clearCompAndResult();
            imc->generateMessage(WM_IME_STARTCOMPOSITION, 0, 0);
        }

        if (cs.compReadStr.size() < maxCompLen - 1) {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            cs.compReadStr.insert(readCursorPos, charCode);
            cs.compReadAttr.insert(readCursorPos, ATTR_INPUT);

            readToComp(&cs);

            cs.setCursorPos(cs.compStr.size() - backCursorPos);
            cs.setDeltaStart(0);

            madeChanges = true;
        } else {
            MessageBeep((UINT)-1);
        }
    } else if (virtKey == VK_BACK) {
        if (cs.compReadStr.size() != 0) {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            if (readCursorPos > 0) {
                cs.compReadStr.erase(readCursorPos - 1);
                cs.compReadAttr.erase(readCursorPos - 1);

                readToComp(&cs);

                cs.setCursorPos(cs.compStr.size() - backCursorPos);
                cs.setDeltaStart(0);

                madeChanges = true;
            }
        }
    } else if (virtKey == VK_DELETE) {
        if (cs.compReadStr.size() != 0) {
            DWORD backCursorPos = cs.compStr.size() - cs.cursorPos();
            DWORD readCursorPos = cs.compReadStr.size() - backCursorPos;

            if (readCursorPos < cs.compReadStr.size()) {
                cs.compReadStr.erase(readCursorPos);
                cs.compReadAttr.erase(readCursorPos);

                readToComp(&cs);

                cs.setDeltaStart(0);

                madeChanges = true;
            }
        }
    } else if (virtKey == VK_LEFT) {
        if (cs.compStr.size() != 0) {
            if (cs.cursorPos() > 0) {
                compToRead(&cs);

                cs.setDeltaStart(cs.cursorPos() - 1);
                cs.setCursorPos(cs.cursorPos() - 1);

                madeChanges = true;
            }
        }
    } else if (virtKey == VK_RIGHT) {
        if (cs.compStr.size() != 0) {
            if (cs.cursorPos() < cs.compStr.size()) {
                compToRead(&cs);

                cs.setDeltaStart(cs.cursorPos());
                cs.setCursorPos(cs.cursorPos() + 1);

                madeChanges = true;
            }
        }
    } else if (virtKey == VK_HOME) {
        if (cs.compStr.size() != 0) {
            if (cs.cursorPos() > 0) {
                compToRead(&cs);

                cs.setDeltaStart(0);
                cs.setCursorPos(0);

                madeChanges = true;
            }
        }
    } else if (virtKey == VK_END) {
        if (cs.compStr.size() != 0) {
            if (cs.cursorPos() < cs.compStr.size()) {
                compToRead(&cs);

                cs.setDeltaStart(cs.cursorPos());
                cs.setCursorPos(cs.compStr.size());

                madeChanges = true;
            }
        }
    }

    if (cs.compStr.size() != 0) {
        if (madeChanges) {
            imc->generateMessage(
                    WM_IME_COMPOSITION,
                    0,
                    GCS_COMP_READ_ALL | GCS_COMP_ALL |
                        GCS_CURSORPOS | GCS_DELTASTART
                    );
        }
    } else {
        cancelComp(imc, &cs);
    }
}
