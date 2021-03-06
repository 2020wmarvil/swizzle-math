#pragma once

#include <iostream>
#include <cmath>

#include "property.hpp"

namespace cm {

// https://newbedev.com/how-to-achieve-vector-swizzling-in-c

#pragma region SCALAR
template<typename T, unsigned int I>
struct scalar_swizzle {
    T v[1];

    #pragma region SCALAR OVERLOADS
    T &operator=(const T x) {
        v[I] = x;
        return v[I];
    }

    operator T() const {
        return v[I];
    }

    T operator++(int) {
        return v[I]++;
    }

    T operator++() {
        return ++v[I];
    }

    T operator--(int) {
        return v[I]--;
    }

    T operator--() {
        return --v[I];
    }

    scalar_swizzle& operator+=(const scalar_swizzle& right) {
      v[I] += right;
      return *this;
    }
    scalar_swizzle& operator+=(const T& right) {
      v[I] += right;
      return *this;
    }
    friend scalar_swizzle operator+(scalar_swizzle left, const scalar_swizzle& right) {
      left += right;
      return left;
    }

    scalar_swizzle& operator-=(const scalar_swizzle& right) {
      v[I] -= right;
      return *this;
    }
    scalar_swizzle& operator-=(const T& right) {
      v[I] -= right;
      return *this;
    }
    friend scalar_swizzle operator-(scalar_swizzle left, const scalar_swizzle& right) {
      left -= right;
      return left;
    }

    scalar_swizzle& operator*=(const scalar_swizzle& right) {
      v[I] *= right;
      return *this;
    }
    scalar_swizzle& operator*=(const T& right) {
      v[I] *= right;
      return *this;
    }
    friend scalar_swizzle operator*(scalar_swizzle left, const scalar_swizzle& right) {
      left *= right;
      return left;
    }

    scalar_swizzle& operator/=(const scalar_swizzle& right) {
      v[I] /= right;
      return *this;
    }
    scalar_swizzle& operator/=(const T& right) {
      v[I] /= right;
      return *this;
    }
    friend scalar_swizzle operator/(scalar_swizzle left, const scalar_swizzle& right) {
      left /= right;
      return left;
    }
    #pragma endregion
};
#pragma endregion

#pragma region VEC2
template<typename vec_type, typename T, unsigned int A, unsigned int B>
struct vec2_swizzle {
    T d[2];
    vec_type operator=(const vec_type& vec) {
        return vec_type(d[A] = vec.x, d[B] = vec.y);
    }

    operator vec_type() {
        return vec_type(d[A], d[B]);
    }

    vec2_swizzle& operator*=(const T& right) {
      d[0] *= right;
      d[1] *= right;
      return *this;
    }
    vec2_swizzle& operator/=(const T& right) {
      d[0] /= right;
      d[1] /= right;
      return *this;
    }
};

template<typename T>
struct vec2 {
    float k;
    #pragma region SWIZZLES
    union {
        T d[2];
        scalar_swizzle<T, 0> x, i;
        scalar_swizzle<T, 1> y, j;

        vec2_swizzle<vec2, T, 0, 0> xx;
        vec2_swizzle<vec2, T, 0, 1> xy;
        vec2_swizzle<vec2, T, 1, 0> yx;
        vec2_swizzle<vec2, T, 1, 1> yy;
    };
    #pragma endregion

    #pragma region PROPERTIES
    class : public ReadOnlyProperty<float> { 
    public:
        vec2<T>* _vec;

        void SetVec(vec2<T>* vec) { _vec = vec; }

        // really weird memory issues here if we try and access _vec->x or _vec->y more than once
        virtual operator T () const { 
            T x = _vec->x; T y = _vec->y;
            return (T)std::sqrt(x*x + y*y); 
        }
    } magnitude;

    class : public ReadOnlyProperty<vec2<T>> { 
    public:
        vec2<T>* _vec;

        void SetVec(vec2<T>* vec) { _vec = vec; }

        virtual operator vec2<T> () const {
            T x = _vec->x; T y = _vec->y;
            T mag = std::sqrt(x*x + y*y); 
            return vec2<T>(x, y) / mag;
        }
    } normalized;
    #pragma endregion

    vec2() : vec2(0, 0) { }
    vec2(T all) : vec2(all, all) { }
    vec2(T a, T b) {
        x = a; y = b;
        magnitude.SetVec(this);
        normalized.SetVec(this);
    }

    #pragma region OPERATOR OVERLOADS
    vec2& operator+=(const vec2& right) {
      x += right.x;
      y += right.y;
      return *this;
    }
    friend vec2 operator+(vec2 left, const vec2& right) {
      left += right;
      return left;
    }
    vec2& operator-=(const vec2& right) {
      x += right.x;
      y += right.y;
      return *this;
    }
    friend vec2 operator-(vec2 left, const vec2& right) {
      left += right;
      return left;
    }
    vec2& operator*=(const vec2& right) {
      x *= right.x;
      y *= right.y;
      return *this;
    }
    vec2& operator*=(const T& right) {
      x *= right;
      y *= right;
      return *this;
    }
    friend vec2 operator*(vec2 left, const vec2& right) {
      left *= right;
      return left;
    }
    friend vec2 operator*(vec2 left, const T& right) {
      left *= right;
      return left;
    }
    vec2& operator/=(const vec2& right) {
      x /= right.x;
      y /= right.y;
      return *this;
    }
    friend vec2 operator/(vec2 left, const vec2& right) {
      left /= right;
      return left;
    }
    friend vec2 operator/(vec2 left, const T& right) {
      left /= right;
      return left;
    }
    vec2 operator-() {
      return vec2<T>(-x, -y);
    }
    #pragma endregion
};
#pragma endregion


    template<typename T>
    inline bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    template<typename T>
    inline bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs) {
        return !operator==(lhs,rhs);
    }

    template<typename vec_type, typename T, unsigned int A, unsigned int B, unsigned int C, unsigned int D>
    inline bool operator==(const vec2_swizzle<vec_type, T, A, B>& lhs, const vec2_swizzle<vec_type, T, C, D>& rhs) {
        return lhs.d[0] == rhs.d[0] && lhs.d[1] == rhs.d[1];
    }

#pragma region PRINT OVERLOADS
#define DEFINE_PRINT_FUNCS(X) \
template<typename T> \
inline std::ostream& operator<<(std::ostream &os, X<T>      vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<double>     vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<uint8_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<int8_t>		vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<uint16_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<int16_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<uint32_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<int32_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<uint64_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; } \
inline std::ostream& operator<<(std::ostream &os, X<int64_t>	vec) { os << "(" << vec.x << ", " << vec.y << ")"; return os; }

DEFINE_PRINT_FUNCS(vec2)
#pragma endregion
}
