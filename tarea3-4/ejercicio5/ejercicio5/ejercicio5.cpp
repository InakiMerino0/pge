// ejercicio5.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "ejercicio5.h"
#include <string>
#include <windows.h>
#include <windowsx.h>  // ← define GET_X_LPARAM / GET_Y_LPARAM


#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
static int  g_piso = 0;    // último piso solicitado (0 = ninguno)
static int  g_pressed = -1;   // índice del botón resaltado (0..2, -1 = ninguno)
static RECT g_btn[3];         // layout de los 3 rectángulos

// Calcula layout en 3 columnas con margen
static void ComputeLayout(HWND hWnd) {
    RECT rc; GetClientRect(hWnd, &rc);
    const int margin = 16;
    const int gap = 12;
    int w = rc.right - rc.left - 2 * margin - 2 * gap;
    int h = rc.bottom - rc.top - 2 * margin - 60;     // deja espacio para la leyenda abajo
    int col = w / 3;
    int top = rc.top + margin;
    int left = rc.left + margin;

    for (int i = 0; i < 3; ++i) {
        g_btn[i].left = left + i * (col + gap);
        g_btn[i].top = top;
        g_btn[i].right = g_btn[i].left + col;
        g_btn[i].bottom = top + (h > 120 ? 120 : h); // alto fijo cómodo
    }
}


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
    LoadStringW(hInstance, IDC_EJERCICIO5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO5));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EJERCICIO5);
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

        // Actualiza layout según tamaño actual
        ComputeLayout(hWnd);

        // Brochas de color
        HBRUSH normal = CreateSolidBrush(RGB(230, 230, 230));
        HBRUSH pressed = CreateSolidBrush(RGB(173, 216, 230)); // light blue
        HBRUSH frame = (HBRUSH)GetStockObject(BLACK_BRUSH);

        // Fuente grande
        HFONT big = CreateFontW(-48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HGDIOBJ oldF = SelectObject(hdc, big);

        // Dibuja los 3 “botones”
        for (int i = 0; i < 3; ++i) {
            HBRUSH b = (i == g_pressed ? pressed : normal);
            FillRect(hdc, &g_btn[i], b);
            FrameRect(hdc, &g_btn[i], (HBRUSH)frame);

            // Etiqueta centrada “1/2/3”
            wchar_t num[2] = { (wchar_t)(L'1' + i), 0 };
            RECT r = g_btn[i];
            DrawTextW(hdc, num, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        // Leyenda: “Último piso: X”
        SelectObject(hdc, oldF);
        DeleteObject(big);

        HFONT small = CreateFontW(-22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        oldF = SelectObject(hdc, small);

        RECT rc; GetClientRect(hWnd, &rc);
        rc.top = g_btn[0].bottom + 24;
        wchar_t status[64];
        wsprintfW(status, L"\x00DAltimo piso: %d", g_piso);  // “Último piso: N”
        DrawTextW(hdc, status, -1, &rc, DT_CENTER | DT_TOP | DT_SINGLELINE);

        // Limpieza
        SelectObject(hdc, oldF);
        DeleteObject(small);
        DeleteObject(normal);
        DeleteObject(pressed);

        EndPaint(hWnd, &ps);
        return 0;
    }

        break;
    case WM_LBUTTONDOWN: {
        // Coordenadas del click en cliente
        POINT p{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        // Asegura layout actualizado y verifica hit-test
        ComputeLayout(hWnd);
        for (int i = 0; i < 3; ++i) {
            if (PtInRect(&g_btn[i], p)) {
                g_piso = i + 1;        // 1..3
                g_pressed = i;         // resaltar este botón
                InvalidateRect(hWnd, nullptr, TRUE);
                break;
            }
        }
        return 0;
    }

                       // OPCIONAL: clic derecho resta (sin menos de 0) y quita highlight
    case WM_RBUTTONDOWN: {
        if (g_piso > 0) --g_piso;
        g_pressed = -1;
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }

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
