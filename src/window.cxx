// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/window.hxx"

#include <debug/tracing/macros.hxx>

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef min
#undef min
#endif

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef max
#undef max
#endif

#include <logging/logging.hxx>

namespace logenium {

Window::WindowKind Window::GetKind() const { return kind; }

Window::Window(WindowKind kind) : kind(kind) {
    ZoneScoped;
    log::trace("Window initialized");
}

Window::~Window() {
    ZoneScoped;
    log::trace("Window destroyed");
}

Window::NativeHandle &Window::GetNativeHandle() { return native_handle; }

Window::State &Window::GetState() { return state; }

}  // namespace logenium
