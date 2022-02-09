#pragma once
#include <array>
#include <cstdint>

namespace detail {
    template <typename value_t, size_t n>
    struct vec_members {
        std::array<value_t, n> array;
        constexpr value_t &operator[](size_t i) noexcept {
            return array[i];
        }
        constexpr const value_t &operator[](size_t i) const noexcept {
            return array[i];
        }
    };
    template <typename value_t>
    struct vec_members<value_t, 2> {
        value_t x, y;
        constexpr value_t &operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            default: return x;
            }
        }
        constexpr const value_t &operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            default: return x;
            }
        }
    };
    template <typename value_t>
    struct vec_members<value_t, 3> {
        value_t x, y, z;
        constexpr value_t &operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            default: return x;
            }
        }
        constexpr const value_t &operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            default: return x;
            }
        }
    };
    template <typename value_t>
    struct vec_members<value_t, 4> {
        value_t x, y, z, w;
        constexpr value_t &operator[](size_t i) noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: return x;
            }
        }
        constexpr const value_t &operator[](size_t i) const noexcept {
            switch (i) {
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: return x;
            }
        }
    };
} // namespace detail

template <typename value_t, size_t n>
struct vec : detail::vec_members<value_t, n> {
    using vec_type = vec<value_t, n>;

    constexpr auto operator+(const vec_type &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] + other[i];
        return result;
    }
    constexpr auto operator-(const vec_type &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] - other[i];
        return result;
    }
    constexpr auto operator*(const vec_type &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] * other[i];
        return result;
    }
    constexpr auto operator/(const vec_type &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] / other[i];
        return result;
    }
    constexpr auto operator*(const value_t &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] * other;
        return result;
    }
    constexpr auto operator/(const value_t &other) const {
        vec_type result;
        for (size_t i = 0; i < n; ++i)
            result[i] = (*this)[i] / other;
        return result;
    }
    constexpr auto &operator+=(const vec_type &other) {
        return (*this = *this + other);
    }
    constexpr auto &operator-=(const vec_type &other) {
        return (*this = *this - other);
    }
    constexpr auto &operator*=(const value_t &other) { return (*this = *this * other); }
    constexpr auto &operator/=(const value_t &other) { return (*this = *this / other); }
    constexpr auto dot(const vec_type &other) const {
        value_t result = 0;
        for (size_t i = 0; i < n; ++i)
            result += (*this)[i] * other[i];
        return result;
    }
};

template <typename value_t, size_t n>
inline auto length(const vec<value_t, n> &v) {
    return sqrt(v.dot(v));
}

template <typename value_t, size_t n>
inline auto normalize(const vec<value_t, n> &v) {
    return v / length(v);
}
