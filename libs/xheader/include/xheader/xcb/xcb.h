// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_XCB_XCB_H
#define LOGENIUM_XHEADER_XCB_XCB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <xcb/xcb.h>  // IWYU pragma: export

#else  // __linux__

#include <stdint.h>

#define XCB_COPY_FROM_PARENT 0L

typedef struct xcb_connection_t {
    int dummy;
} xcb_connection_t;

typedef struct {
    uint8_t response_type;
    uint8_t pad0;
    uint16_t sequence;
    uint32_t pad[7];
    uint32_t full_sequence;
} xcb_generic_event_t;

typedef struct {
    unsigned int sequence;
} xcb_void_cookie_t;

#include <xheader/xcb/xproto.h>  // as seen in xcb/xcb.h include after typedef xcb_void_cookie_t

xcb_connection_t *xcb_connect(const char *displayname, int *screenp);
int xcb_connection_has_error(xcb_connection_t *c);
const struct xcb_setup_t *xcb_get_setup(xcb_connection_t *c);
void xcb_disconnect(xcb_connection_t *c);
uint32_t xcb_generate_id(xcb_connection_t *c);
int xcb_flush(xcb_connection_t *c);
xcb_generic_event_t *xcb_wait_for_event(xcb_connection_t *c);

#endif  // __linux__

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_XCB_XCB_H
