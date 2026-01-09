// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/window.hxx"

#include <debug/tracing/macros.hxx>

namespace logenium {

Window::WindowKind Window::GetKind() const { return kind; }

Window::Window(WindowKind kind) : kind(kind) { ZoneScoped; }

Window::NativeHandle &Window::GetNativeHandle() { return native_handle; }

}  // namespace logenium
