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
