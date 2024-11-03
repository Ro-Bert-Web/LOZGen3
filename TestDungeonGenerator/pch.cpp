// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
std::unordered_map<KVal, wchar_t> kValChars = { {T, L'T'}, {F, L'F'}, {X, L'X'} };
KVal kVals[] = { T, F, X };
