#ifndef LOGENIUM_XHEADER_WINDOWS_H
#define LOGENIUM_XHEADER_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

// clang-format off
#include <Windows.h> // IWYU pragma: export
// clang-format on

#include <debugapi.h>      // IWYU pragma: export
#include <libloaderapi.h>  // IWYU pragma: export
#include <minwindef.h>     // IWYU pragma: export
#include <windef.h>        // IWYU pragma: export
#include <winuser.h>       // IWYU pragma: export

#else

#include <stdint.h>
#include <wchar.h>

typedef void VOID;
typedef unsigned int UINT;
typedef long LONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef void *PVOID;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef PVOID HANDLE;
typedef const char *LPCSTR;
typedef const wchar_t *LPCWSTR;
typedef char *LPSTR;
typedef wchar_t *LPWSTR;
typedef LONG LRESULT;
typedef uintptr_t UINT_PTR;
typedef intptr_t LONG_PTR;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef WORD ATOM;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TRUE 1
#define FALSE 0

struct HINSTANCE__ {
    int dummy;
};

struct HWND__ {
    int dummy;
};

struct HICON__ {
    int dummy;
};

struct HBRUSH__ {
    int dummy;
};

struct HMENU__ {
    int dummy;
};

typedef struct HINSTANCE__ *HINSTANCE;
typedef HINSTANCE HMODULE;
typedef struct HWND__ *HWND;
typedef struct HICON__ *HICON;
typedef HICON HCURSOR;
typedef struct HBRUSH__ *HBRUSH;
typedef struct HMENU__ *HMENU;

typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSEXA {
    UINT cbSize;
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCSTR lpszMenuName;
    LPCSTR lpszClassName;
    HICON hIconSm;
} WNDCLASSEXA;

typedef struct tagWNDCLASSEXW {
    UINT cbSize;
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCWSTR lpszMenuName;
    LPCWSTR lpszClassName;
    HICON hIconSm;
} WNDCLASSEXW;

HMODULE GetModuleHandleA(LPCSTR /* lpModuleName */);
HMODULE GetModuleHandleW(LPCWSTR /* lpModuleName */);
HICON LoadIconA(HINSTANCE /* hInstance */, LPCSTR /* lpIconName */);
HICON LoadIconW(HINSTANCE /* hInstance */, LPCWSTR /* lpIconName */);
HCURSOR LoadCursorA(HINSTANCE /* hInstance */, LPCSTR /* lpCursorName */);
HCURSOR LoadCursorW(HINSTANCE /* hInstance */, LPCWSTR /* lpCursorName */);
LRESULT DefWindowProcA(HWND /* hWnd */, UINT /* msg */, WPARAM /* wParam */, LPARAM /* lParam */);
LRESULT DefWindowProcW(HWND /* hWnd */, UINT /* msg */, WPARAM /* wParam */, LPARAM /* lParam */);
ATOM RegisterClassExA(const WNDCLASSEXA *);
ATOM RegisterClassExW(const WNDCLASSEXW *);
BOOL UnregisterClassA(LPCSTR /* lpClassName */, HINSTANCE /* hInstance */);
BOOL UnregisterClassW(LPCWSTR /* lpClassName */, HINSTANCE /* hInstance */);
HWND CreateWindowExA(DWORD /* dwExStyle */, LPCSTR /* lpClassName */, LPCSTR /* lpWindowName */, DWORD /* dwStyle */,
                     int /* x */, int /* y */, int /* nWidth */, int /* nHeight */, HWND /* hWndParent */,
                     HMENU /* hMenu */, HINSTANCE /* hInstance */, LPVOID /* lpParam */);
HWND CreateWindowExW(DWORD /* dwExStyle */, LPCWSTR /* lpClassName */, LPCWSTR /* lpWindowName */, DWORD /* dwStyle */,
                     int /* x */, int /* y */, int /* nWidth */, int /* nHeight */, HWND /* hWndParent */,
                     HMENU /* hMenu */, HINSTANCE /* hInstance */, LPVOID /* lpParam */);
BOOL DestroyWindow(HWND /* hWnd */);
BOOL ShowWindow(HWND /* hWnd */, int /* nCmdShow */);
BOOL UpdateWindow(HWND /* hWnd */);
VOID DebugBreak();
BOOL IsDebuggerPresent();

constexpr UINT CS_VREDRAW = 0x0001;
constexpr UINT CS_HREDRAW = 0x0002;

#define IDI_APPLICATION ((LPCSTR)(uintptr_t)32512)

#define IDC_ARROW ((LPCSTR)(uintptr_t)32512)

constexpr int COLOR_WINDOW = 5;

constexpr DWORD WS_OVERLAPPED = 0x00000000L;
constexpr DWORD WS_CAPTION = 0x00C00000L;
constexpr DWORD WS_SYSMENU = 0x00080000L;
constexpr DWORD WS_THICKFRAME = 0x00040000L;
constexpr DWORD WS_MINIMIZEBOX = 0x00020000L;
constexpr DWORD WS_MAXIMIZEBOX = 0x00010000L;
constexpr DWORD WS_OVERLAPPEDWINDOW =
    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

constexpr DWORD WS_EX_WINDOWEDGE = 0x00000100L;
constexpr DWORD WS_EX_CLIENTEDGE = 0x00000200L;
constexpr DWORD WS_EX_OVERLAPPEDWINDOW = (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE);

constexpr int SW_HIDE = 0;
constexpr int SW_SHOWNORMAL = 1;
constexpr int SW_NORMAL = 1;
constexpr int SW_SHOWMINIMIZED = 2;
constexpr int SW_SHOWMAXIMIZED = 3;
constexpr int SW_MAXIMIZE = 3;
constexpr int SW_SHOWNOACTIVATE = 4;
constexpr int SW_SHOW = 5;
constexpr int SW_MINIMIZE = 6;
constexpr int SW_SHOWMINNOACTIVE = 7;
constexpr int SW_SHOWNA = 8;
constexpr int SW_RESTORE = 9;

constexpr int CW_USEDEFAULT = (int)0x80000000;

#ifdef UNICODE

#define GetModuleHandle GetModuleHandleW
#define LoadIcon LoadIconW
#define LoadCursor LoadCursorW
#define DefWindowProc DefWindowProcW
#define RegisterClassEx RegisterClassExW
#define UnregisterClass UnregisterClassW
#define CreateWindowEx CreateWindowExW

typedef WNDCLASSEXW WNDCLASSEX;

#else

#define GetModuleHandle GetModuleHandleA
#define LoadIcon LoadIconA
#define LoadCursor LoadCursorA
#define DefWindowProc DefWindowProcA
#define RegisterClassEx RegisterClassExA
#define UnregisterClass UnregisterClassA
#define CreateWindowEx CreateWindowExA

typedef WNDCLASSEXA WNDCLASSEX;

#endif

#endif  // defined(_WIN32)

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_WINDOWS_H
