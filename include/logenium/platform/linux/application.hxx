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
    ~LinuxApplication() override;

    static bool classof(const Application *app);
    static std::unique_ptr<LinuxApplication> Create();

  protected:
    LinuxApplication(ApplicationKind kind);
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_APPLICATION_HXX
