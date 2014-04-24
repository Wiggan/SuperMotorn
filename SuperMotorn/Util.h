#pragma once
#include <iostream>
#include <vector>
class Util {
public:
    static std::wstring                 string2wstring(const std::string& pString);
    static std::string                  wstring2string(const std::wstring& pWString);
    static std::wstring                 searchDirectory(const std::wstring &pDirectory, const std::wstring &pFileName);
    static std::wstring                 findDirectory(const std::wstring &pStartDirectory, const std::wstring &pFileName);
    static std::vector<std::wstring>    listFilesInDirectory(const std::wstring &pDirectory);
    static std::wstring                 stripPath(const std::wstring &pPath);
};

