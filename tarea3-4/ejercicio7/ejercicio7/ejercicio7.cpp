// ejercicio7.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "ejercicio7.h"
#include <deque>
#include <string>
#include <sstream>
#include <windowsx.h>   // GET_X_LPARAM, GET_Y_LPARAM

struct Item { std::wstring texto; };
static std::deque<Item> g_log;      // buffer circular (máx 10)

static void push_log(std::wstring s) {
    g_log.push_front({ std::move(s) });
    if (g_log.size() > 10) g_log.pop_back();
}


#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EJERCICIO7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO7));

    MSG msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EJERCICIO7);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT : {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        // Título
        HFONT hTitle = CreateFontW(-22, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HGDIOBJ old = SelectObject(hdc, hTitle);
        RECT rc; GetClientRect(hWnd, &rc);
        RECT rt = rc; rt.left += 16; rt.top += 12;
        DrawTextW(hdc, L"Ultimos eventos (reciente arriba):", -1, &rt,
            DT_LEFT | DT_SINGLELINE | DT_TOP);

        // Lista monoespaciada para alinear
        SelectObject(hdc, old); DeleteObject(hTitle);
        HFONT mono = CreateFontW(-18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            FIXED_PITCH | FF_MODERN, L"Consolas");
        old = SelectObject(hdc, mono);

        int y = rt.top + 30;
        for (const auto& it : g_log) {
            RECT r{ rt.left, y, rc.right - 16, y + 22 };
            DrawTextW(hdc, it.texto.c_str(), -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
            y += 22;
        }

        SelectObject(hdc, old); DeleteObject(mono);
        EndPaint(hWnd, &ps);
        return 0;
    }

        break;
    case WM_CHAR: {
        wchar_t ch = (wchar_t)wParam;
        std::wstringstream ss;
        if (ch == L'\b') ss << L"WM_CHAR  '\\b' (backspace)";
        else             ss << L"WM_CHAR  '" << ch << L"'";
        push_log(ss.str());
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }
        break;
    case WM_LBUTTONDOWN: {
        POINT p{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        std::wstringstream ss;
        ss << L"WM_LBUTTONDOWN  (x=" << p.x << L", y=" << p.y << L")";
        push_log(ss.str());
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }
        break;
    case WM_SIZE: {
        int w = LOWORD(lParam), h = HIWORD(lParam);
        std::wstringstream ss;
        ss << L"WM_SIZE  " << w << L"\u00D7" << h; // “×”
        push_log(ss.str());
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
