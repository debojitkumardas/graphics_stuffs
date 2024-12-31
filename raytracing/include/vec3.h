#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <concepts>
#include <ostream>

template <typename T>
concept Numerical = std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, double>;

template <Numerical T>
class vec3 {
public:
    T e[3];

    vec3()
    : e{0.0, 0.0, 0.0} {}

    vec3(T e0, T e1, T e2)
    : e{e0, e1, e2} {}

    T x() const { return e[0]; }
    T y() const { return e[1]; }
    T z() const { return e[2]; }

    vec3<T> operator-() const {
        return vec3(-e[0], -e[1], -e[2]);
    }

    T operator[](size_t i) const {
        return e[i];
    }

    T& operator[](size_t i) {
        return e[i];
    }

    vec3<T>& operator+=(const vec3<T> v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vec3<T>& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    vec3<T>& operator/=(double t) {
        return *this *= 1/t;
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length() const {
        return std::sqrt(length_squared());
    }
};

using vec3d_T = vec3<double>;
using point3 = vec3<double>;
using color = vec3<double>;

// vector utility functions

template <Numerical T>
std::ostream& operator<<(std::ostream& out, const vec3<T>& v) {
    return out << '(' << v.e[0] << ',' << v.e[1] << ',' << v.e[2] << ')';
}

template <Numerical T>
vec3<T> operator+(const vec3<T>& u, const vec3<T>& v) {
    return vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template <Numerical T>
vec3<T> operator-(const vec3<T>& u, const vec3<T>& v) {
    return vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template <Numerical T>
vec3<T> operator*(const vec3<T>& u, const vec3<T>& v) {
    return vec3<T>(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

template <Numerical T>
vec3<T> operator*(double t, const vec3<T>& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

template <Numerical T>
vec3<T> operator/(double t, const vec3<T>& v) {
    return (1/t) * v;
}

template <Numerical T>
T dot(const vec3<T>& u, const vec3<T>& v) {
    return (u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2]);
}

template <Numerical T>
T cross(const vec3<T>& u, const vec3<T>& v) {
    return vec3(u.e[1]*v.e[2] - u.e[2]*v.e[2],
                u.e[2]*v.e[0] - u.e[0]*v.e[2],
                u.e[0]*v.e[1] - u.e[1]*v.e[0]);
}

template <Numerical T>
vec3<T> unit_vector(const vec3<T>& v) {
    return v / v.length();
}

// color utility functions
inline void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // translate the [0,1] component values to the byte range [0,255]
    int rbyte = static_cast<int>(255.999 * r);
    int gbyte = static_cast<int>(255.999 * g);
    int bbyte = static_cast<int>(255.999 * b);

    // write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif // !VEC3_H
