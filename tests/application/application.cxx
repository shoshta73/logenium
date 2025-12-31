// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting.hxx>

#include "logenium/application.hxx"
#include "logenium/platform/linux/X11/application.hxx"
#include "logenium/platform/linux/application.hxx"
#include "logenium/platform/linux/wayland/application.hxx"
#include "logenium/platform/windows/application.hxx"

using namespace logenium;
using namespace corelib;

namespace {

class MockApplication : public Application {
  public:
    explicit MockApplication(ApplicationKind kind) : Application(kind) {}

    void Run() override {}

    static bool classof(const Application *app) {
        auto k = app->GetKind();
        return k >= ApplicationKind::AK_Windows && k <= ApplicationKind::AK_LinuxX11;
    }

  protected:
    void RegisterWindowClass() override {}
    void UnregisterWindowClass() override {}
};

class MockWindowsApplication : public Application {
  public:
    MockWindowsApplication() : Application(ApplicationKind::AK_Windows) {}

    void Run() override {}

    static bool classof(const Application *app) { return app->GetKind() == ApplicationKind::AK_Windows; }

  protected:
    void RegisterWindowClass() override {}
    void UnregisterWindowClass() override {}
};

class MockLinuxApplication : public Application {
  public:
    explicit MockLinuxApplication(ApplicationKind kind) : Application(kind) {}

    void Run() override {}

    static bool classof(const Application *app) {
        auto k = app->GetKind();
        return k >= ApplicationKind::AK_Linux && k <= ApplicationKind::AK_LinuxX11;
    }

  protected:
    void RegisterWindowClass() override {}
    void UnregisterWindowClass() override {}
};

class MockX11Application : public Application {
  public:
    MockX11Application() : Application(ApplicationKind::AK_LinuxX11) {}

    void Run() override {}

    static bool classof(const Application *app) { return app->GetKind() == ApplicationKind::AK_LinuxX11; }

  protected:
    void RegisterWindowClass() override {}
    void UnregisterWindowClass() override {}
};

class MockWaylandApplication : public Application {
  public:
    MockWaylandApplication() : Application(ApplicationKind::AK_LinuxWayland) {}

    void Run() override {}

    static bool classof(const Application *app) { return app->GetKind() == ApplicationKind::AK_LinuxWayland; }

  protected:
    void RegisterWindowClass() override {}
    void UnregisterWindowClass() override {}
};

}  // namespace

TEST(ApplicationTest, ApplicationKindEnumValues) {
    EXPECT_EQ(static_cast<std::uint8_t>(Application::ApplicationKind::AK_Windows), 0);
    EXPECT_EQ(static_cast<std::uint8_t>(Application::ApplicationKind::AK_Linux), 1);
    EXPECT_EQ(static_cast<std::uint8_t>(Application::ApplicationKind::AK_LinuxWayland), 2);
    EXPECT_EQ(static_cast<std::uint8_t>(Application::ApplicationKind::AK_LinuxX11), 3);
}

TEST(ApplicationTest, GetKindWindowsApplication) {
    MockWindowsApplication app;
    EXPECT_EQ(app.GetKind(), Application::ApplicationKind::AK_Windows);
}

TEST(ApplicationTest, GetKindLinuxApplication) {
    MockLinuxApplication app(Application::ApplicationKind::AK_Linux);
    EXPECT_EQ(app.GetKind(), Application::ApplicationKind::AK_Linux);
}

TEST(ApplicationTest, GetKindX11Application) {
    MockX11Application app;
    EXPECT_EQ(app.GetKind(), Application::ApplicationKind::AK_LinuxX11);
}

TEST(ApplicationTest, GetKindWaylandApplication) {
    MockWaylandApplication app;
    EXPECT_EQ(app.GetKind(), Application::ApplicationKind::AK_LinuxWayland);
}

TEST(ApplicationTest, ClassofWindowsApplication) {
    MockWindowsApplication win_app;
    const Application *app = &win_app;

    EXPECT_TRUE(MockWindowsApplication::classof(app));
    EXPECT_FALSE(MockLinuxApplication::classof(app));
    EXPECT_FALSE(MockX11Application::classof(app));
    EXPECT_FALSE(MockWaylandApplication::classof(app));
}

TEST(ApplicationTest, ClassofLinuxApplication) {
    MockLinuxApplication linux_app(Application::ApplicationKind::AK_Linux);
    const Application *app = &linux_app;

    EXPECT_FALSE(MockWindowsApplication::classof(app));
    EXPECT_TRUE(MockLinuxApplication::classof(app));
    EXPECT_FALSE(MockX11Application::classof(app));
    EXPECT_FALSE(MockWaylandApplication::classof(app));
}

TEST(ApplicationTest, ClassofX11Application) {
    MockX11Application x11_app;
    const Application *app = &x11_app;

    EXPECT_FALSE(MockWindowsApplication::classof(app));
    EXPECT_TRUE(MockLinuxApplication::classof(app));
    EXPECT_TRUE(MockX11Application::classof(app));
    EXPECT_FALSE(MockWaylandApplication::classof(app));
}

TEST(ApplicationTest, ClassofWaylandApplication) {
    MockWaylandApplication wayland_app;
    const Application *app = &wayland_app;

    EXPECT_FALSE(MockWindowsApplication::classof(app));
    EXPECT_TRUE(MockLinuxApplication::classof(app));
    EXPECT_FALSE(MockX11Application::classof(app));
    EXPECT_TRUE(MockWaylandApplication::classof(app));
}

TEST(ApplicationTest, IsaWindowsApplication) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    EXPECT_TRUE(isa<MockWindowsApplication>(app));
    EXPECT_FALSE(isa<MockLinuxApplication>(app));
    EXPECT_FALSE(isa<MockX11Application>(app));
    EXPECT_FALSE(isa<MockWaylandApplication>(app));
}

TEST(ApplicationTest, IsaLinuxApplication) {
    MockLinuxApplication linux_app(Application::ApplicationKind::AK_Linux);
    Application *app = &linux_app;

    EXPECT_FALSE(isa<MockWindowsApplication>(app));
    EXPECT_TRUE(isa<MockLinuxApplication>(app));
    EXPECT_FALSE(isa<MockX11Application>(app));
    EXPECT_FALSE(isa<MockWaylandApplication>(app));
}

TEST(ApplicationTest, IsaX11Application) {
    MockX11Application x11_app;
    Application *app = &x11_app;

    EXPECT_FALSE(isa<MockWindowsApplication>(app));
    EXPECT_TRUE(isa<MockLinuxApplication>(app));
    EXPECT_TRUE(isa<MockX11Application>(app));
    EXPECT_FALSE(isa<MockWaylandApplication>(app));
}

TEST(ApplicationTest, IsaWaylandApplication) {
    MockWaylandApplication wayland_app;
    Application *app = &wayland_app;

    EXPECT_FALSE(isa<MockWindowsApplication>(app));
    EXPECT_TRUE(isa<MockLinuxApplication>(app));
    EXPECT_FALSE(isa<MockX11Application>(app));
    EXPECT_TRUE(isa<MockWaylandApplication>(app));
}

TEST(ApplicationTest, IsaVariadic) {
    MockX11Application x11_app;
    Application *app = &x11_app;

    EXPECT_TRUE((isa<MockLinuxApplication, MockX11Application>(app)));
    EXPECT_TRUE((isa<MockX11Application, MockWaylandApplication>(app)));
    EXPECT_FALSE((isa<MockWindowsApplication, MockWaylandApplication>(app)));
}

TEST(ApplicationTest, CastWindowsApplication) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = cast<MockWindowsApplication>(app);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, CastX11Application) {
    MockX11Application x11_app;
    Application *app = &x11_app;

    MockX11Application *casted = cast<MockX11Application>(app);
    EXPECT_EQ(casted, &x11_app);

    MockLinuxApplication *linux_casted = cast<MockLinuxApplication>(app);
    EXPECT_EQ(linux_casted, static_cast<Application *>(&x11_app));
}

TEST(ApplicationTest, CastConstPreservation) {
    const MockWindowsApplication win_app;
    const Application *app = &win_app;

    const MockWindowsApplication *casted = cast<MockWindowsApplication>(app);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, DynCastSuccessful) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = dyn_cast<MockWindowsApplication>(app);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, DynCastFailed) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockLinuxApplication *casted = dyn_cast<MockLinuxApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, DynCastX11ToLinux) {
    MockX11Application x11_app;
    Application *app = &x11_app;

    MockLinuxApplication *casted = dyn_cast<MockLinuxApplication>(app);
    EXPECT_NE(casted, nullptr);
}

TEST(ApplicationTest, DynCastOrNullSuccessful) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = dyn_cast_or_null<MockWindowsApplication>(app);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, DynCastOrNullFailed) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockLinuxApplication *casted = dyn_cast_or_null<MockLinuxApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, DynCastOrNullWithNullptr) {
    Application *app = nullptr;

    MockWindowsApplication *casted = dyn_cast_or_null<MockWindowsApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, CastOrNullSuccessful) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = cast_or_null<MockWindowsApplication>(app);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, CastOrNullWithNullptr) {
    Application *app = nullptr;

    MockWindowsApplication *casted = cast_or_null<MockWindowsApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, IsaAndPresentTrue) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    EXPECT_TRUE(isa_and_present<MockWindowsApplication>(app));
}

TEST(ApplicationTest, IsaAndPresentFalse) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    EXPECT_FALSE(isa_and_present<MockLinuxApplication>(app));
}

TEST(ApplicationTest, IsaAndPresentWithNullptr) {
    Application *app = nullptr;

    EXPECT_FALSE(isa_and_present<MockWindowsApplication>(app));
}

TEST(ApplicationTest, CastIfPresentSuccessful) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = cast_if_present<MockWindowsApplication>(app);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, CastIfPresentWithNullptr) {
    Application *app = nullptr;

    MockWindowsApplication *casted = cast_if_present<MockWindowsApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, DynCastIfPresentSuccessful) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockWindowsApplication *casted = dyn_cast_if_present<MockWindowsApplication>(app);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_app);
}

TEST(ApplicationTest, DynCastIfPresentFailed) {
    MockWindowsApplication win_app;
    Application *app = &win_app;

    MockLinuxApplication *casted = dyn_cast_if_present<MockLinuxApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, DynCastIfPresentWithNullptr) {
    Application *app = nullptr;

    MockWindowsApplication *casted = dyn_cast_if_present<MockWindowsApplication>(app);
    EXPECT_EQ(casted, nullptr);
}

TEST(ApplicationTest, NativeHandleDefaultConstructor) {
    Application::NativeHandle handle;
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(ApplicationTest, NativeHandleVoidPtrConstructor) {
    void *test_ptr = reinterpret_cast<void *>(0x12345678);
    Application::NativeHandle handle(test_ptr);
    void *ptr = handle;
    EXPECT_EQ(ptr, test_ptr);
}

TEST(ApplicationTest, NativeHandleNullptrConstructor) {
    Application::NativeHandle handle(nullptr);
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(ApplicationTest, StateDefaultValues) {
    Application::State state;
    EXPECT_FALSE(state.is_running);
}

TEST(ApplicationTest, StateModification) {
    Application::State state;
    state.is_running = true;
    EXPECT_TRUE(state.is_running);
}

TEST(ApplicationTest, GetStateAccess) {
    MockWindowsApplication app;
    Application::State &state = app.GetState();
    EXPECT_FALSE(state.is_running);
    state.is_running = true;
    EXPECT_TRUE(app.GetState().is_running);
}

TEST(ApplicationTest, GetNativeHandleAccess) {
    MockWindowsApplication app;
    Application::NativeHandle &handle = app.GetNativeHandle();
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(ApplicationTest, PredicateIsaPred) {
    {
        MockWindowsApplication win_app;
        auto pred = IsaPred<MockWindowsApplication>;
        EXPECT_TRUE(pred(&win_app));
    }
    {
        MockLinuxApplication linux_app(Application::ApplicationKind::AK_Linux);
        auto pred = IsaPred<MockWindowsApplication>;
        EXPECT_FALSE(pred(&linux_app));
    }
}

TEST(ApplicationTest, PredicateIsaAndPresentPred) {
    MockWindowsApplication win_app;
    Application *app_ptr = &win_app;
    Application *null_ptr = nullptr;

    auto pred = IsaAndPresentPred<MockWindowsApplication>;
    EXPECT_TRUE(pred(app_ptr));
    EXPECT_FALSE(pred(null_ptr));
}
