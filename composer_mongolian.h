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

#ifndef COMPOSER_MONGOLIAN_H
#define COMPOSER_MONGOLIAN_H

#include "composer.h"

class ComposerMongolian : public Composer
{
public:
    ComposerMongolian();
    ~ComposerMongolian();

protected:
    virtual void readToComp(CompString* cs);

    // ConversionRule flags
    static const DWORD x_m  = 0x00010000u; // only for male words
    static const DWORD x_f  = 0x00020000u; // only for female words
    static const DWORD x_mm = 0x00040000u; // make the word male
    static const DWORD x_mf = 0x00080000u; // make the word female
};

#endif // COMPOSER_MONGOLIAN_H
