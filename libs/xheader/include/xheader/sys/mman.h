// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_SYS_MMAN_H
#define LOGENIUM_XHEADER_SYS_MMAN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <sys/mman.h>  // IWYU pragma: export

#else

// memfd_create flags
#define MFD_CLOEXEC 0x0001U
#define MFD_ALLOW_SEALING 0x0002U

// mmap protection flags
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_NONE 0x0

// mmap flags
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_FAILED ((void *)-1)

int memfd_create(const char *name, unsigned int flags);
void *mmap(void *addr, size_t length, int prot, int flags, int fd, int64_t offset);
int munmap(void *addr, size_t length);

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_SYS_MMAN_H
