#include "FDS_RigidBody_Component.h"
#include "engine/component/FDS_Transform_Component.h"
#include "engine/object/FDS_GameObject.h"
#include "engine/core/FDS_Context.h"

#include "box2d/math_functions.h"
#include "box2d/id.h"
#include "spdlog/spdlog.h"

#include <cmath>

namespace fds
{
    RigidBodyComponent::RigidBodyComponent(
        PhysicsEngine& physics_engine,
        b2BodyType body_type,
        float density,
        float friction,
        float restitution)
        : physics_engine_(&physics_engine)
        , body_type_(body_type)
        , default_density_(density)
        , default_friction_(friction)
        , default_restitution_(restitution)
    {
        // 注意：body_id_ 在 init() 中创建，因为需要 TransformComponent
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
        clean();
    }

    void RigidBodyComponent::init()
    {
        if (!owner_)
        {
            spdlog::error("RigidBodyComponent::init: owner is null");
            return;
        }

        // 获取TransformComponent
        transform_ = owner_->getComponent<TransformComponent>();
        if (!transform_)
        {
            spdlog::warn("RigidBodyComponent::init: TransformComponent not found, creating default");
            transform_ = owner_->addComponent<TransformComponent>();
        }

        if (!physics_engine_ || B2_IS_NULL(physics_engine_->getWorldId()))
        {
            spdlog::error("RigidBodyComponent::init: PhysicsEngine is invalid");
            return;
        }

        // 创建Box2D物体定义
        b2BodyDef body_def = b2DefaultBodyDef();
        body_def.type = body_type_;

        // 将像素坐标转换为米
        glm::vec2 position_pixels = transform_->getPosition();
        glm::vec2 position_meters = physics_engine_->pixelsToMeters(position_pixels);
        body_def.position = {position_meters.x, position_meters.y};

        // 将角度转换为旋转
        float angle_rad = transform_->getRotation();
        body_def.rotation = b2MakeRot(angle_rad);

        // 创建物体
        body_id_ = b2CreateBody(physics_engine_->getWorldId(), &body_def);
        
        if (B2_IS_NULL(body_id_))
        {
            spdlog::error("RigidBodyComponent::init: Failed to create Box2D body");
            return;
        }

        // 设置用户数据为GameObject指针（用于碰撞回调）
        b2Body_SetUserData(body_id_, owner_);

        spdlog::debug("RigidBodyComponent::init: Body created for GameObject '{}'", owner_->getName());
    }

    void RigidBodyComponent::update(float delta_time, fds::Context& context)
    {
        if (B2_IS_NULL(body_id_) || !transform_)
            return;

        // 根据物体类型决定同步方向
        if (body_type_ == b2_dynamicBody)
        {
            // 动态物体：物理驱动Transform
            if (sync_physics_to_transform_)
            {
                syncPhysicsToTransform();
            }
        }
        else if (body_type_ == b2_kinematicBody)
        {
            // 运动学物体：Transform驱动物理
            if (sync_transform_to_physics_)
            {
                syncTransformToPhysics();
            }
        }
        else // b2_staticBody
        {
            // 静态物体：Transform驱动物理（通常只在初始化时）
            if (sync_transform_to_physics_)
            {
                syncTransformToPhysics();
            }
        }
    }

    void RigidBodyComponent::clean()
    {
        if (B2_IS_NON_NULL(body_id_))
        {
            b2DestroyBody(body_id_);
            body_id_ = b2_nullBodyId;
            spdlog::debug("RigidBodyComponent::clean: Body destroyed");
        }
    }

    void RigidBodyComponent::syncTransformToPhysics()
    {
        if (B2_IS_NULL(body_id_) || !transform_)
            return;

        glm::vec2 position_pixels = transform_->getPosition();
        glm::vec2 position_meters = physics_engine_->pixelsToMeters(position_pixels);
        float angle_rad = transform_->getRotation();
        b2Rot rotation = b2MakeRot(angle_rad);

        b2Body_SetTransform(body_id_, {position_meters.x, position_meters.y}, rotation);
    }

    void RigidBodyComponent::syncPhysicsToTransform()
    {
        if (B2_IS_NULL(body_id_) || !transform_)
            return;

        b2Transform b2_transform = b2Body_GetTransform(body_id_);
        glm::vec2 position_meters = {b2_transform.p.x, b2_transform.p.y};
        glm::vec2 position_pixels = physics_engine_->metersToPixels(position_meters);
        
        float angle_rad = b2Atan2(b2_transform.q.s, b2_transform.q.c);

        transform_->setPosition(position_pixels);
        transform_->setRotation(angle_rad);
    }

    void RigidBodyComponent::addCircleShape(float radius, const glm::vec2& offset, bool is_sensor)
    {
        if (B2_IS_NULL(body_id_))
        {
            spdlog::error("RigidBodyComponent::addCircleShape: Body not initialized");
            return;
        }

        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = (body_type_ == b2_dynamicBody) ? default_density_ : 0.0f;
        shape_def.material.friction = default_friction_;
        shape_def.material.restitution = default_restitution_;
        shape_def.isSensor = is_sensor;

        b2Circle circle;
        float radius_meters = physics_engine_->pixelsToMeters(radius);
        glm::vec2 offset_meters = physics_engine_->pixelsToMeters(offset);
        circle.radius = radius_meters;
        circle.center = {offset_meters.x, offset_meters.y};

        b2ShapeId shape_id = b2CreateCircleShape(body_id_, &shape_def, &circle);
        if (B2_IS_NULL(shape_id))
        {
            spdlog::error("RigidBodyComponent::addCircleShape: Failed to create circle shape");
        }
    }

    void RigidBodyComponent::addBoxShape(const glm::vec2& size, const glm::vec2& offset, bool is_sensor)
    {
        if (B2_IS_NULL(body_id_))
        {
            spdlog::error("RigidBodyComponent::addBoxShape: Body not initialized");
            return;
        }

        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = (body_type_ == b2_dynamicBody) ? default_density_ : 0.0f;
        shape_def.material.friction = default_friction_;
        shape_def.material.restitution = default_restitution_;
        shape_def.isSensor = is_sensor;

        // 创建矩形多边形（Box2D使用多边形表示矩形）
        glm::vec2 half_size = size * 0.5f;
        glm::vec2 half_size_meters = physics_engine_->pixelsToMeters(half_size);
        glm::vec2 offset_meters = physics_engine_->pixelsToMeters(offset);

        b2Polygon polygon;
        polygon.count = 4;
        polygon.vertices[0] = {-half_size_meters.x + offset_meters.x, -half_size_meters.y + offset_meters.y};
        polygon.vertices[1] = {half_size_meters.x + offset_meters.x, -half_size_meters.y + offset_meters.y};
        polygon.vertices[2] = {half_size_meters.x + offset_meters.x, half_size_meters.y + offset_meters.y};
        polygon.vertices[3] = {-half_size_meters.x + offset_meters.x, half_size_meters.y + offset_meters.y};

        b2ShapeId shape_id = b2CreatePolygonShape(body_id_, &shape_def, &polygon);
        if (B2_IS_NULL(shape_id))
        {
            spdlog::error("RigidBodyComponent::addBoxShape: Failed to create box shape");
        }
    }

    void RigidBodyComponent::addPolygonShape(const glm::vec2* vertices, int vertex_count, bool is_sensor)
    {
        if (B2_IS_NULL(body_id_))
        {
            spdlog::error("RigidBodyComponent::addPolygonShape: Body not initialized");
            return;
        }

        if (vertex_count < 3 || vertex_count > B2_MAX_POLYGON_VERTICES)
        {
            spdlog::error("RigidBodyComponent::addPolygonShape: Invalid vertex count: {}", vertex_count);
            return;
        }

        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = (body_type_ == b2_dynamicBody) ? default_density_ : 0.0f;
        shape_def.material.friction = default_friction_;
        shape_def.material.restitution = default_restitution_;
        shape_def.isSensor = is_sensor;

        b2Polygon polygon;
        polygon.count = vertex_count;
        for (int i = 0; i < vertex_count; ++i)
        {
            glm::vec2 vertex_meters = physics_engine_->pixelsToMeters(vertices[i]);
            polygon.vertices[i] = {vertex_meters.x, vertex_meters.y};
        }

        b2ShapeId shape_id = b2CreatePolygonShape(body_id_, &shape_def, &polygon);
        if (B2_IS_NULL(shape_id))
        {
            spdlog::error("RigidBodyComponent::addPolygonShape: Failed to create polygon shape");
        }
    }

    void RigidBodyComponent::setLinearVelocity(const glm::vec2& velocity)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        // 将像素/秒转换为米/秒
        glm::vec2 velocity_meters = physics_engine_->pixelsToMeters(velocity);
        b2Body_SetLinearVelocity(body_id_, {velocity_meters.x, velocity_meters.y});
    }

    glm::vec2 RigidBodyComponent::getLinearVelocity() const
    {
        if (B2_IS_NULL(body_id_))
            return {0.0f, 0.0f};
        
        b2Vec2 velocity_meters = b2Body_GetLinearVelocity(body_id_);
        return physics_engine_->metersToPixels({velocity_meters.x, velocity_meters.y});
    }

    void RigidBodyComponent::setAngularVelocity(float angular_velocity)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        b2Body_SetAngularVelocity(body_id_, angular_velocity);
    }

    float RigidBodyComponent::getAngularVelocity() const
    {
        if (B2_IS_NULL(body_id_))
            return 0.0f;
        
        return b2Body_GetAngularVelocity(body_id_);
    }

    void RigidBodyComponent::applyForceToCenter(const glm::vec2& force)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        // 力通常以牛顿为单位，这里假设输入是像素单位的力
        // 需要根据实际需求调整转换
        glm::vec2 force_meters = physics_engine_->pixelsToMeters(force);
        b2Body_ApplyForceToCenter(body_id_, {force_meters.x, force_meters.y}, true);
    }

    void RigidBodyComponent::applyForce(const glm::vec2& force, const glm::vec2& point)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        glm::vec2 force_meters = physics_engine_->pixelsToMeters(force);
        glm::vec2 point_meters = physics_engine_->pixelsToMeters(point);
        b2Body_ApplyForce(body_id_, {force_meters.x, force_meters.y}, {point_meters.x, point_meters.y}, true);
    }

    void RigidBodyComponent::applyLinearImpulseToCenter(const glm::vec2& impulse)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        glm::vec2 impulse_meters = physics_engine_->pixelsToMeters(impulse);
        b2Body_ApplyLinearImpulseToCenter(body_id_, {impulse_meters.x, impulse_meters.y}, true);
    }

    void RigidBodyComponent::applyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        glm::vec2 impulse_meters = physics_engine_->pixelsToMeters(impulse);
        glm::vec2 point_meters = physics_engine_->pixelsToMeters(point);
        b2Body_ApplyLinearImpulse(body_id_, {impulse_meters.x, impulse_meters.y}, {point_meters.x, point_meters.y}, true);
    }

    void RigidBodyComponent::setBodyType(b2BodyType type)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        body_type_ = type;
        b2Body_SetType(body_id_, type);
    }

    void RigidBodyComponent::setAwake(bool awake)
    {
        if (B2_IS_NULL(body_id_))
            return;
        
        b2Body_SetAwake(body_id_, awake);
    }

    bool RigidBodyComponent::isAwake() const
    {
        if (B2_IS_NULL(body_id_))
            return false;
        
        return b2Body_IsAwake(body_id_);
    }
}

