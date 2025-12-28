// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_DLFCN_H
#define LOGENIUM_XHEADER_DLFCN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <dlfcn.h>  // IWYU pragma: export

#else

// dlopen flags
#define RTLD_LAZY 0x00001
#define RTLD_NOW 0x00002
#define RTLD_GLOBAL 0x00100
#define RTLD_LOCAL 0x00000

// TODO: Implement no throw marker
void *dlopen(const char *__file, int __mode);

// TODO: Implement no throw marker
int dlclose(void *__handle);

#endif  // __linux__

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_DLFCN_H
