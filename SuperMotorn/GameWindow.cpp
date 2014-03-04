#include <winsock2.h>
#include "Game.h"
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <WinDef.h>
#include <fstream>
#include "Timer.h"
HWND window = 0;
Game* game = NULL;
int width = 1200;
int height = 900;
bool keyStatus[91];
Timer timer;
bool paused = false;
LRESULT CALLBACK 
WndProc(HWND hWnd, UINT pMessage, WPARAM wParam, LPARAM lParam) {
    switch ( pMessage ) {
    case WM_LBUTTONDOWN:
        //MessageBox(0, L"Why hello there", 0, 0);
        return 0;
    case WM_KEYDOWN:
        switch ( wParam ) {
        case VK_ESCAPE:
            DestroyWindow(window);
            break;
        case VK_PAUSE:
            paused = !paused;
            if ( paused ) {
                timer.stop();
            } else {
                timer.start();
            }
            break;
        case VK_F4:
            game->nextCamera();
            break;
        case VK_F8:
            game->toggleFps();
            break;
        case VK_F6:
            game->debug();
            break;
        case VK_F5:
            timer.stop();
            delete game;
            game = new Game(window, width, height, &timer);
            timer.reset();
            ZeroMemory(&keyStatus, sizeof(keyStatus));
            break;
        default:
            if ( wParam < 91 && !keyStatus[wParam] ) {
                game->keyDown(wParam);
                keyStatus[wParam] = true;
            }
        }
        return 0;
    case WM_KEYUP:
        if ( wParam < 91 && keyStatus[wParam] ) {
            game->keyUp(wParam);
            keyStatus[wParam] = false;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        return 0;
    }
    return DefWindowProc(hWnd, pMessage, wParam, lParam);
}
void 
CreateConsole() {
    AllocConsole();
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
    long lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    FILE *fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );
    std::ios::sync_with_stdio();
}
int 
Run() {
    MSG msg = { 0 };
    if ( game == NULL ) {
        game = new Game(window, width, height, &timer);
        timer.reset();
        ZeroMemory(&keyStatus, sizeof(keyStatus));
    }
    while ( msg.message != WM_QUIT ) {
        if ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if ( !paused ) {
                timer.tick();
                game->tick(timer.getDeltaTime());
            } else {
                Sleep(100);
            }
        }
    }
    delete game;
    return (int)msg.wParam;
}
int WINAPI 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {
    CreateConsole();
    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = 0;
    wc.lpszClassName    = L"SuperMotorn";
    if ( !RegisterClass(&wc) ) {
        MessageBox(0, L"Failed registering class", 0, 0);
        exit(-1);
    }
    window = CreateWindow(
        L"SuperMotorn",
        L"SuperMotorn",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);
    if ( window == 0 ) {
        MessageBox(0, L"Failed creating window", 0, 0);
        exit(-1);
    }
    
    ShowWindow(window, nShowCmd);
    UpdateWindow(window);
    return Run();
}