# Box2D 物理系统使用指南

## 概述

FDS_2D 引擎封装了 Box2D 物理引擎，提供了简单易用的物理组件系统。物理系统会自动处理碰撞检测、刚体动力学和物理模拟。

## 核心组件

### PhysicsEngine
物理引擎管理器，负责创建和管理 Box2D 世界。

### RigidBodyComponent
刚体组件，将 GameObject 与 Box2D 物理物体关联，自动同步位置和旋转。

## 基本使用

### 1. 创建动态物体（受重力影响）

```cpp
// 在 Scene::init() 中
auto ball = std::make_unique<fds::GameObject>("Ball");

// 添加 TransformComponent（位置）
ball->addComponent<fds::TransformComponent>(glm::vec2(400, 100));

// 添加 RigidBodyComponent（物理）
auto* rigidBody = ball->addComponent<fds::RigidBodyComponent>(
    context.getPhysicsEngine(),
    b2_dynamicBody,  // 动态物体
    1.0f,            // 密度
    0.2f,            // 摩擦系数
    0.5f             // 弹性系数
);

// 添加圆形碰撞体
rigidBody->addCircleShape(25.0f);  // 半径25像素

// 添加 SpriteComponent（渲染）
ball->addComponent<fds::SpriteComponent>(
    "ball_texture",
    context.getResourceManager()
);

scene.addGameObject(std::move(ball));
```

### 2. 创建静态物体（地面、墙壁）

```cpp
auto ground = std::make_unique<fds::GameObject>("Ground");

ground->addComponent<fds::TransformComponent>(glm::vec2(400, 500));

auto* rigidBody = ground->addComponent<fds::RigidBodyComponent>(
    context.getPhysicsEngine(),
    b2_staticBody  // 静态物体
);

// 添加矩形碰撞体（地面）
rigidBody->addBoxShape(glm::vec2(800, 50));  // 宽800，高50像素

scene.addGameObject(std::move(ground));
```

### 3. 创建运动学物体（平台、移动障碍）

```cpp
auto platform = std::make_unique<fds::GameObject>("Platform");

platform->addComponent<fds::TransformComponent>(glm::vec2(200, 300));

auto* rigidBody = platform->addComponent<fds::RigidBodyComponent>(
    context.getPhysicsEngine(),
    b2_kinematicBody  // 运动学物体
);

rigidBody->addBoxShape(glm::vec2(200, 20));

// 在 update() 中手动控制位置
// 运动学物体会自动同步到物理系统
```

### 4. 应用力和冲量

```cpp
// 应用力（持续作用）
rigidBody->applyForceToCenter(glm::vec2(100, 0));  // 向右推

// 应用冲量（瞬间作用）
rigidBody->applyLinearImpulseToCenter(glm::vec2(0, -500));  // 向上跳

// 设置速度
rigidBody->setLinearVelocity(glm::vec2(100, 0));  // 向右移动
```

### 5. 传感器（触发器）

传感器不会产生物理碰撞，但会触发碰撞事件：

```cpp
auto trigger = std::make_unique<fds::GameObject>("Trigger");

trigger->addComponent<fds::TransformComponent>(glm::vec2(400, 200));

auto* rigidBody = trigger->addComponent<fds::RigidBodyComponent>(
    context.getPhysicsEngine(),
    b2_staticBody
);

// 创建传感器（is_sensor = true）
rigidBody->addCircleShape(50.0f, glm::vec2(0, 0), true);
```

## 坐标系统

Box2D 使用**米**作为单位，而游戏通常使用**像素**。物理引擎会自动进行转换：

- 默认转换比例：100 像素 = 1 米
- 可通过 `PhysicsEngine::setPixelsPerMeter()` 调整

## 物体类型

- **b2_staticBody**: 静态物体，不受力影响，用于地面、墙壁等
- **b2_kinematicBody**: 运动学物体，不受力影响，但可以手动设置位置
- **b2_dynamicBody**: 动态物体，受力和重力影响，用于可移动物体

## 同步机制

### 动态物体（Dynamic）
- 物理驱动 Transform：物理模拟的结果会自动更新 GameObject 的位置和旋转

### 运动学物体（Kinematic）
- Transform 驱动物理：手动设置的位置会自动同步到物理系统

### 静态物体（Static）
- Transform 驱动物理：通常只在初始化时同步

## 注意事项

1. **单位转换**：Box2D 使用米，游戏使用像素，引擎会自动转换
2. **时间步长**：物理模拟使用固定时间步长（60 FPS），确保稳定性
3. **性能**：避免创建过多的小物体，合理使用静态物体
4. **传感器**：传感器不产生碰撞响应，只用于检测重叠
5. **形状限制**：多边形最多支持 8 个顶点（B2_MAX_POLYGON_VERTICES）

## 示例场景

```cpp
void MyScene::init()
{
    // 创建地面
    auto ground = std::make_unique<fds::GameObject>("Ground");
    ground->addComponent<fds::TransformComponent>(glm::vec2(400, 550));
    auto* groundBody = ground->addComponent<fds::RigidBodyComponent>(
        context_.getPhysicsEngine(),
        b2_staticBody
    );
    groundBody->addBoxShape(glm::vec2(800, 50));
    addGameObject(std::move(ground));

    // 创建多个小球
    for (int i = 0; i < 10; ++i)
    {
        auto ball = std::make_unique<fds::GameObject>("Ball" + std::to_string(i));
        ball->addComponent<fds::TransformComponent>(
            glm::vec2(100 + i * 50, 100)
        );
        auto* ballBody = ball->addComponent<fds::RigidBodyComponent>(
            context_.getPhysicsEngine(),
            b2_dynamicBody,
            1.0f, 0.2f, 0.8f  // 高弹性
        );
        ballBody->addCircleShape(20.0f);
        addGameObject(std::move(ball));
    }
}
```

## 高级功能

### 自定义物理参数

```cpp
// 设置重力
context.getPhysicsEngine().setGravity(glm::vec2(0, 500));  // 向下

// 设置像素到米的转换比例
context.getPhysicsEngine().setPixelsPerMeter(50.0f);  // 50像素=1米
```

### 多边形碰撞体

```cpp
glm::vec2 vertices[] = {
    {0, 0},
    {50, 0},
    {50, 30},
    {25, 50},
    {0, 30}
};
rigidBody->addPolygonShape(vertices, 5);
```

## 调试建议

1. 使用 `spdlog` 查看物理引擎日志
2. 检查物体是否正确创建（body_id 不为 0）
3. 验证坐标转换是否正确
4. 确保在 `Scene::init()` 中创建物理物体

