#ifndef FDS_PHYSICS_ENGINE_H
#define FDS_PHYSICS_ENGINE_H

#include "engine/utility/FDS_math.h"

#include "glm/vec2.hpp"
#include "box2d/box2d.h"

#include <vector>
#include <utility>
#include <optional>

namespace fds
{
    class GameObject;

    /// 物理引擎，封装Box2D世界
    /// 负责管理物理世界的创建、更新和销毁
    class PhysicsEngine
    {
    private:
        b2WorldId world_id_ = b2_nullWorldId;
        float pixels_per_meter_ = 100.0f;  // 像素到米的转换比例，默认100像素=1米
        float time_accumulator_ = 0.0f;    // 时间累积器，用于固定时间步长
        const float fixed_time_step_ = 1.0f / 60.0f;  // 固定时间步长（60 FPS）
        const int velocity_iterations_ = 6;  // 速度迭代次数
        const int position_iterations_ = 2;  // 位置迭代次数

    public:
        /// 创建物理引擎，初始化Box2D世界
        /// @param gravity 重力向量（像素/秒²），默认向下 (0, 980)
        explicit PhysicsEngine(const glm::vec2& gravity = {0.0f, 980.0f});
        ~PhysicsEngine();

        PhysicsEngine(const PhysicsEngine &) = delete;
        PhysicsEngine &operator=(const PhysicsEngine &) = delete;
        PhysicsEngine(PhysicsEngine &&) = delete;
        PhysicsEngine &operator=(PhysicsEngine &&) = delete;

        /// 更新物理模拟
        /// @param delta_time 帧时间（秒）
        void update(float delta_time);

        /// 获取Box2D世界ID
        b2WorldId getWorldId() const { return world_id_; }

        /// 设置像素到米的转换比例
        /// @param pixels_per_meter 每米对应的像素数，默认100
        void setPixelsPerMeter(float pixels_per_meter) { pixels_per_meter_ = pixels_per_meter; }

        /// 获取像素到米的转换比例
        float getPixelsPerMeter() const { return pixels_per_meter_; }

        /// 将像素坐标转换为米（Box2D单位）
        float pixelsToMeters(float pixels) const { return pixels / pixels_per_meter_; }
        glm::vec2 pixelsToMeters(const glm::vec2& pixels) const { return pixels / pixels_per_meter_; }

        /// 将米（Box2D单位）转换为像素坐标
        float metersToPixels(float meters) const { return meters * pixels_per_meter_; }
        glm::vec2 metersToPixels(const glm::vec2& meters) const { return meters * pixels_per_meter_; }

        /// 设置重力
        void setGravity(const glm::vec2& gravity);

        /// 获取重力
        glm::vec2 getGravity() const;

    private:
        /// 初始化Box2D世界
        void initWorld(const glm::vec2& gravity);
    };

}

#endif // FDS_PHYSICS_ENGINE_H