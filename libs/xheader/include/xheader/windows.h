#ifndef LOGENIUM_XHEADER_WINDOWS_H
#define LOGENIUM_XHEADER_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

// clang-format off
#include <Windows.h> // IWYU pragma: export
// clang-format on

#include <libloaderapi.h>  // IWYU pragma: export
#include <minwindef.h>     // IWYU pragma: export

#else

#include <wchar.h>

typedef const char *LPCSTR;
typedef const wchar_t *LPCWSTR;

struct HINSTANCE__ {
    int dummy;
};

typedef struct HINSTANCE__ *HINSTANCE;
typedef HINSTANCE HMODULE;

HMODULE GetModuleHandleA(LPCSTR /* lpModuleName */);
HMODULE GetModuleHandleW(LPCWSTR /* lpModuleName */);

#ifdef UNICODE

#define GetModuleHandle GetModuleHandleW

#else

#define GetModuleHandle GetModuleHandleA

#endif

#endif  // defined(_WIN32)

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_WINDOWS_H
