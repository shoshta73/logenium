// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_AUTO_RELEASE_HXX
#define LOGENIUM_CORELIB_UTILITY_AUTO_RELEASE_HXX

#include <concepts>
#include <functional>
#include <memory>

namespace corelib {

/**
 * @ingroup utility
 * @brief RAII wrapper for automatic resource management with custom releasers.
 *
 * AutoRelease manages resources that require custom cleanup logic, automatically
 * calling the releaser function when the object is destroyed or reset. This is
 * useful for C-style handles, file descriptors, or other resources that don't
 * follow RAII patterns natively.
 *
 * @tparam T The type of the resource to manage
 * @tparam InvalidValue The sentinel value indicating an invalid/uninitialized resource
 *                      (default: value-initialized T{})
 *
 * @note This class is move-only (not copyable)
 * @note The releaser is only called if the resource value != InvalidValue
 * @note Move operations are noexcept and leave the moved-from object in a valid state
 *
 * Example:
 * @code
 * // Managing a file descriptor
 * AutoRelease<int, -1> fd(open("file.txt", O_RDONLY), [](int fd) { close(fd); });
 * if (fd) {
 *     // Use fd as an int
 *     read(fd, buffer, size);
 * }
 * // fd is automatically closed when it goes out of scope
 *
 * // Managing a Windows HANDLE
 * AutoRelease<HANDLE, nullptr> handle(
 *     CreateFile(...),
 *     [](HANDLE h) { CloseHandle(h); }
 * );
 *
 * // Managing a custom resource
 * AutoRelease<char*, nullptr> str(
 *     strdup("hello"),
 *     [](char* s) { free(s); }
 * );
 * @endcode
 */
template <typename T, T InvalidValue = {}>
class AutoRelease {
  public:
    /**
     * @brief Default constructor, creates an AutoRelease with an invalid resource.
     */
    AutoRelease() : AutoRelease(InvalidValue, nullptr) {};

    /**
     * @brief Construct an AutoRelease with a resource and releaser function.
     *
     * @param obj The resource to manage
     * @param releaser The function to call when releasing the resource
     */
    AutoRelease(T obj, std::function<void(T)> releaser) : obj(obj), releaser(releaser) {}

    /**
     * @brief Destructor that releases the resource if valid.
     *
     * Calls the releaser function if obj != InvalidValue and releaser is set.
     */
    ~AutoRelease() {
        if ((obj != InvalidValue) && releaser) {
            releaser(obj);
        }
    }

    /**
     * @brief Deleted copy constructor (move-only semantics).
     */
    AutoRelease(const AutoRelease &) = delete;

    /**
     * @brief Deleted copy assignment operator (move-only semantics).
     */
    AutoRelease &operator=(const AutoRelease &) = delete;

    /**
     * @brief Move constructor that transfers ownership from another AutoRelease.
     *
     * @param other The AutoRelease to move from (left in valid but unspecified state)
     */
    AutoRelease(AutoRelease &&other) noexcept : AutoRelease() { Swap(other); }

    /**
     * @brief Move assignment operator that transfers ownership from another AutoRelease.
     *
     * Releases the current resource (if valid) before acquiring the new one.
     *
     * @param other The AutoRelease to move from (left in valid but unspecified state)
     * @return Reference to this object
     */
    auto operator=(AutoRelease &&other) noexcept -> AutoRelease & {
        AutoRelease new_obj{std::move(other)};
        Swap(new_obj);

        return *this;
    }

    /**
     * @brief Swap the contents of this AutoRelease with another.
     *
     * @param other The AutoRelease to swap with
     */
    void Swap(AutoRelease &other) noexcept {
        std::ranges::swap(obj, other.obj);
        std::ranges::swap(releaser, other.releaser);
    }

    /**
     * @brief Release the current resource and acquire a new one.
     *
     * If the current resource is valid, it is released before setting the new value.
     *
     * @param obj The new resource value to manage
     */
    void Reset(T obj) {
        if ((this->obj != InvalidValue) && releaser) {
            releaser(this->obj);
        }
        this->obj = obj;
    }

    /**
     * @brief Get the managed resource value.
     *
     * @return The current resource value
     */
    T Get() const { return obj; }

    /**
     * @brief Implicit conversion to the resource type.
     *
     * Allows using AutoRelease directly where T is expected.
     *
     * @return The current resource value
     */
    operator T() const { return obj; }

    /**
     * @brief Check if the resource is valid (not equal to InvalidValue).
     *
     * @return true if the resource is valid, false otherwise
     */
    explicit operator bool() const { return obj != InvalidValue; }

    /**
     * @brief Get the address of the managed resource.
     *
     * Useful for output parameters in C APIs.
     *
     * @return Pointer to the managed resource
     *
     * Example:
     * @code
     * AutoRelease<HANDLE, nullptr> handle;
     * CreateFileHandle(&handle);  // Uses operator&
     * @endcode
     */
    T *operator&() noexcept { return std::addressof(obj); }

    /**
     * @brief Dereference operator (returns the resource value).
     *
     * @return The current resource value
     */
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
