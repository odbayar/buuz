/*
 * Copyright 2012 Odbayar Nyamtseren <odbayar.n@gmail.com>
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

#ifndef GLOBALS_H
#define GLOBALS_H

// Handle to the instance of our DLL library
extern HINSTANCE moduleInstance;

// Is this a WinLogon process? Set in ImeInquire().
extern bool isWinLogonProcess;

// Window class names for the IME windows
extern const WCHAR uiClassName[];
extern const WCHAR compClassName[];

#endif // GLOBALS_H
