#ifndef FDS_PHYSICS_ENGINE_H
#define FDS_PHYSICS_ENGINE_H

#include "engine/utility/FDS_math.h"

#include "glm/vec2.hpp"

#include <vector>
#include <utility>
#include <optional>

namespace fds
{
    class GameObject;

    class PhysicsEngine
    {
    private:


    public:
        PhysicsEngine() = default;

        PhysicsEngine(const PhysicsEngine &) = delete;
        PhysicsEngine &operator=(const PhysicsEngine &) = delete;
        PhysicsEngine(PhysicsEngine &&) = delete;
        PhysicsEngine &operator=(PhysicsEngine &&) = delete;

        //void update(float delta_time);

    private:

    };

}

#endif // FDS_PHYSICS_ENGINE_H