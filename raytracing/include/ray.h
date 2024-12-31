#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}

    ray(const point3& origin, const vec3d_T& direction)
    : m_orig{origin},
      m_dir{direction} {}

    const point3& origin() const {
        return m_orig;
    }

    const vec3d_T& direction() const {
        return m_dir;
    }

private:
    point3 m_orig;
    vec3d_T m_dir;
};

#endif // !RAY_H
