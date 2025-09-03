// ejercicio8.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "ejercicio8.h"
#include <string>
#include <deque>
#include <cwctype>
#include <windowsx.h>



#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana 
static std::wstring g_nombre;                   // teclado
static int g_clicks = 0;                        // clics
static int g_w = 0, g_h = 0;                    // tamaño cliente
static RECT g_panel[3];                         // layout: 0=Nombre, 1=Entradas, 2=Tamaño

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static void ComputeLayout(HWND hWnd) {
    RECT rc; GetClientRect(hWnd, &rc);
    const int margin = 16, gap = 12;
    int W = rc.right - rc.left - 2 * margin - 2 * gap;
    int H = rc.bottom - rc.top - 2 * margin;

    int colW = W / 3, colH = H;

    for (int i = 0; i < 3; ++i) {
        g_panel[i].left = rc.left + margin + i * (colW + gap);
        g_panel[i].top = rc.top + margin;
        g_panel[i].right = g_panel[i].left + colW;
        g_panel[i].bottom = g_panel[i].top + colH;
    }
}


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
    LoadStringW(hInstance, IDC_EJERCICIO8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EJERCICIO8);
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
    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        ComputeLayout(hWnd);

        // Brochas + fuentes
        HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
        HPEN   pen = CreatePen(PS_SOLID, 1, RGB(60, 60, 60));
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        HFONT hTitle = CreateFontW(-20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HFONT hValue = CreateFontW(-28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HGDIOBJ oldFont = SelectObject(hdc, hTitle);

        auto drawPanel = [&](int i, const wchar_t* titulo, const std::wstring& valor) {
            // fondo + borde
            FillRect(hdc, &g_panel[i], bg);
            Rectangle(hdc, g_panel[i].left, g_panel[i].top, g_panel[i].right, g_panel[i].bottom);

            // título arriba-izq
            RECT rt = g_panel[i]; rt.left += 12; rt.top += 10; rt.bottom = rt.top + 28;
            DrawTextW(hdc, titulo, -1, &rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            // valor centrado
            SelectObject(hdc, hValue);
            RECT rv = g_panel[i]; rv.left += 12; rv.right -= 12; rv.top = rt.bottom + 6;
            DrawTextW(hdc, valor.c_str(), -1, &rv, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(hdc, hTitle);
            };

        // Valores actuales
        wchar_t bufClicks[32]; wsprintfW(bufClicks, L"%d", g_clicks);
        wchar_t bufSize[64];   wsprintfW(bufSize, L"%d × %d", g_w, g_h);

        drawPanel(0, L"Nombre (teclado)", g_nombre);
        drawPanel(1, L"Entradas (clics)", bufClicks);
        drawPanel(2, L"Tamano (W×H)", bufSize);

        // Limpieza
        SelectObject(hdc, oldFont);
        DeleteObject(hTitle); DeleteObject(hValue);
        SelectObject(hdc, oldPen); DeleteObject(pen);
        DeleteObject(bg);
        EndPaint(hWnd, &ps);
        return 0;
    }

        // Teclado → Nombre
    case WM_CHAR: {
        wchar_t ch = (wchar_t)wParam;
        if (ch == L'\b') { if (!g_nombre.empty()) g_nombre.pop_back(); }
        else if (iswprint(ch)) { g_nombre.push_back(ch); }
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }

                // Clic → Entradas (clic en cualquier lado cuenta)
    case WM_LBUTTONDOWN: {
        ++g_clicks;
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }

                       // Tamaño → W×H
    case WM_SIZE: {
        g_w = LOWORD(lParam);
        g_h = HIWORD(lParam);
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
