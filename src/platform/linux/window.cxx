// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/window.hxx"

#include <debug/tracing/macros.hxx>

#include "logenium/window.hxx"

namespace logenium {

LinuxWindow::LinuxWindow(WindowKind kind) : Window(kind) { ZoneScoped; }

LinuxWindow::~LinuxWindow() { ZoneScoped; }

bool LinuxWindow::classof(const Window *win) {
    auto kind = win->GetKind();
    return kind >= WindowKind::WK_Linux && kind <= WindowKind::WK_LinuxX11;
}

}  // namespace logenium
