// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_UNISTD_H
#define LOGENIUM_XHEADER_UNISTD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <unistd.h>  // IWYU pragma: export

#else

int ftruncate(int fd, int64_t length);
int close(int fd);

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_UNISTD_H
