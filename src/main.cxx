// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <debug/assert.hxx>
#include <debug/tracing/macros.hxx>

#include "logenium/application.hxx"

int main() {
    ZoneScoped;

    using namespace logenium;
    auto app = logenium::Application::Create();
    Assert(app != nullptr, "Application is not created");
    app->Run();
}
