#include "Util.h"
#include <winsock2.h>
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
std::wstring
Util::findDirectory(const std::wstring &pStartDirectory, const std::wstring &pFileName) {
    using namespace std;
    WIN32_FIND_DATA file;
    wstring searchString = pStartDirectory + L"/*";
    wstring foundFilePath;
    HANDLE handle = FindFirstFile(searchString.c_str(), &file);
    if ( handle != INVALID_HANDLE_VALUE ) {
        do {
            wstring strTheNameOfTheFile = file.cFileName;
            if ( wcscmp(strTheNameOfTheFile.c_str(), L".") == 0 || wcscmp(strTheNameOfTheFile.c_str(), L"..") == 0 ) {
            } else if ( wcscmp(strTheNameOfTheFile.c_str(), pFileName.c_str()) == 0 ) {
                foundFilePath = pStartDirectory + L"/" + strTheNameOfTheFile;
                break;
            } else if ( file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                foundFilePath = findDirectory(pStartDirectory + L"/" + strTheNameOfTheFile, pFileName);
                if ( !foundFilePath.empty() ) {
                    break;
                }
            }
        } while ( FindNextFile(handle, &file) );
        FindClose( handle );
    }
    return foundFilePath;
}
std::vector<std::wstring>    
Util::listFilesInDirectory(const std::wstring &pDirectory) {
    using namespace std;
    vector<wstring> files;
    WIN32_FIND_DATA file;
    HANDLE handle = FindFirstFile((pDirectory + L"/*").c_str(), &file);
    if ( handle != INVALID_HANDLE_VALUE ) {
        do {
            if ( wcscmp(file.cFileName, L".") == 0 || wcscmp(file.cFileName, L"..") == 0 ) {
            } else {
                files.push_back(file.cFileName);
            }
        } while ( FindNextFile(handle, &file) );
        FindClose( handle );
    }
    return files;
}
std::wstring                 
Util::stripPath(const std::wstring &pPath) {
    return pPath.substr(pPath.find_last_of(L"/") + 1);
}

