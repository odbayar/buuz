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
#include "composer_russian.h"

ComposerRussian::ComposerRussian()
{
    addRule("A"   , "А" , x_ac);
    addRule("B"   , "Б" , x_ac);
    addRule("C"   , "Ц" , x_ac);
    addRule("CH"  , "Ч" , x_ac);
    addRule("D"   , "Д" , x_ac);
    addRule("E"   , "Э" , x_ac);
    addRule("F"   , "Ф" , x_ac);
    addRule("G"   , "Г" , x_ac);
    addRule("H"   , "Х" , x_ac);
    addRule("I"   , "И" , x_ac);
    addRule("J"   , "Й" , x_ac);
    addRule("K"   , "К" , x_ac);
    addRule("KH"  , "Х" , x_ac);
    addRule("L"   , "Л" , x_ac);
    addRule("M"   , "М" , x_ac);
    addRule("N"   , "Н" , x_ac);
    addRule("O"   , "О" , x_ac);
    addRule("P"   , "П" , x_ac);
    addRule("Q"   , "Я" , x_ac);
    addRule("R"   , "Р" , x_ac);
    addRule("S"   , "С" , x_ac);
    addRule("SH"  , "Ш" , x_ac);
    addRule("SHH" , "Щ" , x_ac);
    addRule("T"   , "Т" , x_ac);
    addRule("U"   , "У" , x_ac);
    addRule("V"   , "В" , x_ac);
    addRule("W"   , "В" , x_ac);
    addRule("X"   , "Х" , x_ac);
    addRule("Y"   , "Ы" , x_ac);
    addRule("YA"  , "Я" , x_ac);
    addRule("YE"  , "Е" , x_ac);
    addRule("YO"  , "Ё" , x_ac);
    addRule("YU"  , "Ю" , x_ac);
    addRule("Z"   , "З" , x_ac);
    addRule("ZH"  , "Ж" , x_ac);
    addRule("'"   , "ь" ,    0);
    addRule("''"  , "Ь" ,    0);
    addRule("\""  , "ъ" ,    0);
    addRule("\"\"", "Ъ" ,    0);

    computeInputChars();

#ifdef DEBUG
    exportConversionRules("C:\\buuz_rules_rus.txt");
#endif
}

ComposerRussian::~ComposerRussian()
{
}
