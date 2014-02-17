#include "Util.h"
#include <Windows.h>
#include <string>

std::wstring
Util::string2wstring(const std::string& pString) {
    std::wstring wideString(pString.length(),L' ');
    std::copy(pString.begin(), pString.end(), wideString.begin());
    return wideString;
}
std::string
Util::wstring2string(const std::wstring& pWString) {
    std::string string(pWString.begin(), pWString.end());
    return string;
}
std::wstring
Util::searchDirectory(const std::wstring &pDirectory, const std::wstring &pFileName) {
    using namespace std;
    WIN32_FIND_DATA file;
    wstring searchString = pDirectory + L"/*";
    wstring foundFilePath;
    HANDLE handle = FindFirstFile(searchString.c_str(), &file);
    if ( handle != INVALID_HANDLE_VALUE ) {
        do {
            wstring strTheNameOfTheFile = file.cFileName;
            if ( wcscmp(strTheNameOfTheFile.c_str(), L".") == 0 || wcscmp(strTheNameOfTheFile.c_str(), L"..") == 0 ) {
            } else if ( file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                foundFilePath = searchDirectory(pDirectory + L"/" + strTheNameOfTheFile, pFileName);
                if ( !foundFilePath.empty() ) {
                    break;
                }
            } else {
                if ( strTheNameOfTheFile == pFileName ) {
                    foundFilePath = pDirectory + L"/" + pFileName;
                    break;
                }
            }
        } while ( FindNextFile(handle, &file) );
        FindClose( handle );
    }
    return foundFilePath;
}
