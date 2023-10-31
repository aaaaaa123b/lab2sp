#pragma warning (disable: 28251)

#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>


#ifndef UNICODE
#define UNICODE
#endif 

#define COLS 2
#define ROWS 3

using namespace std;

vector<wstring> strings;

HFONT hFont;

VOID OnPaint(HWND hWnd, LONG clientWidth, LONG clientHeight)
{
    int padding = 10;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    INT colWidth = (INT)(clientWidth / COLS);
    INT* rowHeights = new INT[ROWS + 1];

    RECT cellRect = RECT();
    int maxRowHeight = 0;
    int topRowY = 0;
    int stringIndex = 0;

    for (int row = 0; row < ROWS; row++) {
        maxRowHeight = 10;
        int leftColumnX = 0;
        int rightColumnX = colWidth;

        for (int col = 0; col < COLS; col++) {
            cellRect.left = leftColumnX + padding;
            cellRect.right = rightColumnX - padding;
            cellRect.top = topRowY + padding;
            cellRect.bottom = topRowY + 1000;

            if (stringIndex < strings.size()) {
                wstring text = strings.at(stringIndex);
                text.erase(remove_if(text.begin(), text.end(), iswspace), text.end()); // Удаляем пробелы
                int currRowHeight = DrawTextEx(hdc, const_cast<wchar_t*>(text.c_str()), -1, &cellRect, DT_WORDBREAK | DT_TOP | DT_EDITCONTROL, NULL);



                if (currRowHeight > maxRowHeight) {
                    maxRowHeight = currRowHeight;
                }

                stringIndex++;
            }

            leftColumnX += colWidth;
            rightColumnX += colWidth;
        }

        if (row == 0) {
            rowHeights[row] = maxRowHeight + padding + padding;
        }
        else {
            rowHeights[row] = rowHeights[row - 1] + maxRowHeight + padding + padding;
        }
        topRowY += maxRowHeight + padding + padding;
    }

    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, clientWidth, 0);

    for (int col = 1; col < COLS; col++)
    {
        MoveToEx(hdc, col * colWidth, 0, NULL);
        LineTo(hdc, col * colWidth, clientHeight);
    }

    for (int row = 0; row < ROWS; row++)
    {
        MoveToEx(hdc, 0, rowHeights[row], NULL);
        LineTo(hdc, clientWidth, rowHeights[row]);
    }

    EndPaint(hWnd, &ps);
}

std::wstring LoadTextFromFile(const std::wstring& filename) {
    std::wifstream file(filename);
    std::wstring text;
    if (file.is_open()) {
        std::wstring line;
        while (std::getline(file, line)) {
            // Удаляем пробелы из строки
            line.erase(std::remove_if(line.begin(), line.end(), iswspace), line.end());
            text += line;
        }
        file.close();
    }
    return text;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    strings.emplace_back(LoadTextFromFile(L"D:\\сп\\lab2\\file1.txt"));
    strings.emplace_back(LoadTextFromFile(L"D:\\сп\\lab2\\file2.txt"));

    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Lab2"),             // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    static LONG  clientWidth = 800;
    static LONG  clientHeight = 600;
 

    switch (message)
    {
    case WM_CREATE:
    {
        
        return 0;
    }
    case WM_PAINT:
    {
    
        OnPaint(hWnd, clientWidth, clientWidth);
  
        return 0;
    }
    
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        clientWidth = LOWORD(lParam);
        clientHeight = HIWORD(lParam);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

