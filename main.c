#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#include "logic.h"
#include "constants.h"

// Global variables
int grid[GRID_X][GRID_Y] = {0};
game_state g_state = INSERT_MODE;
PAINTSTRUCT ps;
HBRUSH hBrush;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("game of life");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nCmdShow
)
{
   WNDCLASSEX wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
   wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName   = NULL;
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

   if (!RegisterClassEx(&wcex))
   {
      MessageBox(NULL,
         _T("Call to RegisterClassEx failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }


   hInst = hInstance;

   HWND hWnd = CreateWindowEx(
      WS_EX_OVERLAPPEDWINDOW,
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      SCREEN_WIDTH, SCREEN_HEIGHT,
      NULL,
      NULL,
      hInstance,
      NULL
   );

   if (!hWnd)
   {
      MessageBox(NULL,
         _T("Call to CreateWindow failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hBrush = CreateSolidBrush(RGB(138, 43, 226));

   //game loop
   MSG msg;

   while (g_state != END_PLS) {

      game_input(&g_state, hWnd);
      if (g_state == RUNNING_MODE) {
         render(grid, hWnd);
      }

      while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
         DispatchMessage(&msg);
         TranslateMessage(&msg);
      }
   }

   return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message) {
      case WM_LBUTTONDOWN: {
         leftButtonTrigger(grid, &hWnd, &lParam, &g_state);
      }
      case WM_PAINT: {
         draw(grid, &hWnd, &hBrush);
         break;
      }
      case WM_DESTROY: {
         PostQuitMessage(0);
         break;
      }
      case WM_CLOSE: {
         g_state = END_PLS;
      }
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}
