// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_AUTO_RELEASE_HXX
#define LOGENIUM_CORELIB_UTILITY_AUTO_RELEASE_HXX

#include <concepts>
#include <functional>
#include <memory>

namespace corelib {

template <typename T, T InvalidValue = {}>
class AutoRelease {
  public:
    AutoRelease() : AutoRelease(InvalidValue, nullptr) {};
    AutoRelease(T obj, std::function<void(T)> releaser) : obj(obj), releaser(releaser) {}

    ~AutoRelease() {
        if ((obj != InvalidValue) && releaser) {
            releaser(obj);
        }
    }

    AutoRelease(const AutoRelease &) = delete;
    AutoRelease &operator=(const AutoRelease &) = delete;

    AutoRelease(AutoRelease &&other) noexcept : AutoRelease() { Swap(other); }
    auto operator=(AutoRelease &&other) noexcept -> AutoRelease & {
        AutoRelease new_obj{std::move(other)};
        Swap(new_obj);

        return *this;
    }

    void Swap(AutoRelease &other) noexcept {
        std::ranges::swap(obj, other.obj);
        std::ranges::swap(releaser, other.releaser);
    }

    void Reset(T obj) {
        if ((this->obj != InvalidValue) && releaser) {
            releaser(this->obj);
        }
        this->obj = obj;
    }

    T Get() const { return obj; }

    operator T() const { return obj; }

    explicit operator bool() const { return obj != InvalidValue; }

    T *operator&() noexcept { return std::addressof(obj); }

    T operator*() const noexcept { return obj; }

  private:
    T obj;
    std::function<void(T)> releaser;
};

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_UTILITY_AUTO_RELEASE_HXX
