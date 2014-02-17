#pragma once
#include <iostream>
class Util {
public:
    static std::wstring            string2wstring(const std::string& pString);
    static std::string             wstring2string(const std::wstring& pWString);
    static std::wstring            searchDirectory(const std::wstring &pDirectory, const std::wstring &pFileName);
};

