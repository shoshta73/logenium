// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/application.hxx"

#include <cstdlib>
#include <cstring>
#include <memory>

#include <xheader/dlfcn.h>

#include <debug/assert.hxx>

#include "logenium/platform/linux/X11/application.hxx"
#include "logenium/platform/linux/wayland/application.hxx"

namespace logenium {

LinuxApplication::LinuxApplication() {
    auto handle = dlopen(nullptr, RTLD_NOW);
    Assert(handle != nullptr, "Failed to self load");
    native_handle = handle;
}

LinuxApplication::~LinuxApplication() {
    auto res = dlclose(native_handle);
    Assert(res == 0, "Failed to self close");
}

std::unique_ptr<LinuxApplication> LinuxApplication::Create() {
    const char *session_type = std::getenv("XDG_SESSION_TYPE");
    Assert(session_type != nullptr, "Failed to get XDG_SESSION_TYPE");

    if (std::strcmp(session_type, "x11") == 0) {
        return std::make_unique<X11Application>();
    } else if (std::strcmp(session_type, "wayland") == 0) {
        return std::make_unique<WaylandApplication>();
    } else {
        Assert(false, "Unsupported session type");
        return nullptr;
    }
}

}  // namespace logenium
