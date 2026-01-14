#ifndef FDS_RIGID_BODY_COMPONENT_H
#define FDS_RIGID_BODY_COMPONENT_H

#include "FDS_Component.h"
#include "engine/physics/FDS_PhysicsEngine.h"

#include "glm/vec2.hpp"
#include "box2d/box2d.h"
#include "box2d/id.h"

#include <optional>
#include <string_view>

namespace fds
{
    class TransformComponent;
    class PhysicsEngine;
    class Context;

    /// 刚体组件，将GameObject与Box2D物理物体关联
    /// 自动同步TransformComponent的位置和旋转
    class RigidBodyComponent final : public Component
    {
        friend class fds::GameObject;

    private:
        PhysicsEngine* physics_engine_ = nullptr;
        TransformComponent* transform_ = nullptr;
        
        b2BodyId body_id_ = b2_nullBodyId;
        b2BodyType body_type_ = b2_dynamicBody;
        
        float default_density_ = 1.0f;
        float default_friction_ = 0.2f;
        float default_restitution_ = 0.0f;
        
        bool sync_transform_to_physics_ = true;  // 是否将Transform同步到物理
        bool sync_physics_to_transform_ = true;  // 是否将物理同步到Transform

    public:
        /// 创建刚体组件
        /// @param physics_engine 物理引擎引用
        /// @param body_type 物体类型：b2_staticBody, b2_kinematicBody, b2_dynamicBody
        /// @param density 默认密度（kg/m²），仅对dynamic body有效，0表示静态质量
        /// @param friction 默认摩擦系数
        /// @param restitution 默认弹性系数（0=不反弹，1=完全弹性）
        RigidBodyComponent(
            PhysicsEngine& physics_engine,
            b2BodyType body_type = b2_dynamicBody,
            float density = 1.0f,
            float friction = 0.2f,
            float restitution = 0.0f
        );

        ~RigidBodyComponent() override;

        RigidBodyComponent(const RigidBodyComponent &) = delete;
        RigidBodyComponent &operator=(const RigidBodyComponent &) = delete;
        RigidBodyComponent(RigidBodyComponent &&) = delete;
        RigidBodyComponent &operator=(RigidBodyComponent &&) = delete;

        /// 添加圆形碰撞体
        /// @param radius 半径（像素）
        /// @param offset 相对于物体中心的偏移（像素）
        /// @param is_sensor 是否为传感器（不产生物理碰撞，只触发事件）
        void addCircleShape(float radius, const glm::vec2& offset = {0.0f, 0.0f}, bool is_sensor = false);

        /// 添加矩形碰撞体
        /// @param size 尺寸（像素）
        /// @param offset 相对于物体中心的偏移（像素）
        /// @param is_sensor 是否为传感器
        void addBoxShape(const glm::vec2& size, const glm::vec2& offset = {0.0f, 0.0f}, bool is_sensor = false);

        /// 添加多边形碰撞体（凸多边形）
        /// @param vertices 顶点数组（像素坐标，相对于物体中心）
        /// @param vertex_count 顶点数量
        /// @param is_sensor 是否为传感器
        void addPolygonShape(const glm::vec2* vertices, int vertex_count, bool is_sensor = false);

        /// 设置线性速度
        void setLinearVelocity(const glm::vec2& velocity);

        /// 获取线性速度
        glm::vec2 getLinearVelocity() const;

        /// 设置角速度（弧度/秒）
        void setAngularVelocity(float angular_velocity);

        /// 获取角速度（弧度/秒）
        float getAngularVelocity() const;

        /// 应用力到物体中心
        void applyForceToCenter(const glm::vec2& force);

        /// 应用力到指定点
        void applyForce(const glm::vec2& force, const glm::vec2& point);

        /// 应用冲量到物体中心
        void applyLinearImpulseToCenter(const glm::vec2& impulse);

        /// 应用冲量到指定点
        void applyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point);

        /// 设置是否将Transform同步到物理（用于手动控制位置）
        void setSyncTransformToPhysics(bool sync) { sync_transform_to_physics_ = sync; }

        /// 设置是否将物理同步到Transform（用于物理驱动位置）
        void setSyncPhysicsToTransform(bool sync) { sync_physics_to_transform_ = sync; }

        /// 获取Box2D物体ID
        b2BodyId getBodyId() const { return body_id_; }

        /// 获取物体类型
        b2BodyType getBodyType() const { return body_type_; }

        /// 设置物体类型
        void setBodyType(b2BodyType type);

        /// 设置是否唤醒物体
        void setAwake(bool awake);

        /// 是否唤醒
        bool isAwake() const;

    private:
        void init() override;
        void update(float delta_time, fds::Context& context) override;
        void clean() override;

        /// 从Transform同步到物理
        void syncTransformToPhysics();

        /// 从物理同步到Transform
        void syncPhysicsToTransform();
    };

}

#endif // FDS_RIGID_BODY_COMPONENT_H

