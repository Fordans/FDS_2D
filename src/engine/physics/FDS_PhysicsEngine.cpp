#include "FDS_PhysicsEngine.h"
#include "engine/component/FDS_Transform_Component.h"
#include "engine/object/FDS_GameObject.h"

#include "spdlog/spdlog.h"
#include "glm/common.hpp"
#include "box2d/math_functions.h"

#include <set>
#include <cmath>

namespace fds
{
    PhysicsEngine::PhysicsEngine(const glm::vec2& gravity)
    {
        initWorld(gravity);
        spdlog::info("PhysicsEngine initialized with gravity: ({}, {})", gravity.x, gravity.y);
    }

    PhysicsEngine::~PhysicsEngine()
    {
        if (world_id_.index != 0)
        {
            b2DestroyWorld(world_id_);
            spdlog::debug("PhysicsEngine world destroyed");
        }
    }

    void PhysicsEngine::initWorld(const glm::vec2& gravity)
    {
        b2WorldDef world_def = b2DefaultWorldDef();
        
        // 将像素单位的重力转换为米单位（Box2D使用米）
        // 假设重力是像素/秒²，需要转换为米/秒²
        // 例如：980 像素/秒² / 100 像素/米 = 9.8 米/秒²
        float pixels_per_meter = pixels_per_meter_;
        world_def.gravity = {gravity.x / pixels_per_meter, gravity.y / pixels_per_meter};
        
        world_def.enableSleep = true;
        world_def.enableContinuous = true;
        world_def.restitutionThreshold = 1.0f;  // 1 m/s
        world_def.hitEventThreshold = 1.0f;     // 1 m/s
        
        world_id_ = b2CreateWorld(&world_def);
        
        if (world_id_.index == 0)
        {
            spdlog::error("Failed to create Box2D world");
            throw std::runtime_error("Failed to create Box2D world");
        }
    }

    void PhysicsEngine::update(float delta_time)
    {
        if (world_id_.index == 0)
            return;

        // 使用固定时间步长进行物理模拟，提高稳定性
        time_accumulator_ += delta_time;
        
        // 限制最大时间步长，防止"spiral of death"
        const float max_time_step = fixed_time_step_ * 2.0f;
        if (time_accumulator_ > max_time_step)
        {
            time_accumulator_ = max_time_step;
        }

        // 执行固定时间步长的物理模拟
        while (time_accumulator_ >= fixed_time_step_)
        {
            b2World_Step(world_id_, fixed_time_step_, velocity_iterations_);
            time_accumulator_ -= fixed_time_step_;
        }
    }

    void PhysicsEngine::setGravity(const glm::vec2& gravity)
    {
        if (world_id_.index == 0)
            return;
        
        float pixels_per_meter = pixels_per_meter_;
        b2Vec2 b2_gravity = {gravity.x / pixels_per_meter, gravity.y / pixels_per_meter};
        b2World_SetGravity(world_id_, b2_gravity);
    }

    glm::vec2 PhysicsEngine::getGravity() const
    {
        if (world_id_.index == 0)
            return {0.0f, 0.0f};
        
        b2Vec2 b2_gravity = b2World_GetGravity(world_id_);
        float pixels_per_meter = pixels_per_meter_;
        return {b2_gravity.x * pixels_per_meter, b2_gravity.y * pixels_per_meter};
    }
}