#ifndef FDS_MATH_H
#define FDS_MATH_H

#include "glm/glm.hpp"

namespace fds
{
    struct Rect
    {
        glm::vec2 size;
        glm::vec2 pos;
    };

    struct FColor
    {
        float r;
        float g;
        float b;
        float a;
    };
}

#endif // FDS_MATH_H