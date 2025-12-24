// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_APPLICATION_HXX
#define LOGENIUM_PLATFORM_LINUX_APPLICATION_HXX

#include <memory>

#include "logenium/application.hxx"

namespace logenium {

class LinuxApplication : public Application {
  public:
    LinuxApplication();
    ~LinuxApplication();

    static std::unique_ptr<LinuxApplication> Create();
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_APPLICATION_HXX
