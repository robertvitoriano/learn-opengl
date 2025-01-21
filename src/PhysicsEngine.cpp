#include "PhysicsEngine.hpp"

void PhysicsEngine::addObject(const RigidBody& body) {
    objects.push_back(body);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& body : objects) {
        // Apply gravity
        body.acceleration = gravity;

        // Update velocity: v = v0 + a * t
        body.velocity += body.acceleration * deltaTime;

        // Update position: p = p0 + v * t
        body.position += body.velocity * deltaTime;
    }
}

void PhysicsEngine::detectAndResolveCollisions() {
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            RigidBody& bodyA = objects[i];
            RigidBody& bodyB = objects[j];

            // Check for collision using spherical bounding volumes
            glm::vec3 diff = bodyA.position - bodyB.position;
            float distance = glm::length(diff);
            float minDistance = bodyA.radius + bodyB.radius;

            if (distance < minDistance) {
                // Resolve collision by moving objects apart
                glm::vec3 collisionNormal = glm::normalize(diff);
                float overlap = minDistance - distance;

                // Adjust positions based on overlap
                bodyA.position += collisionNormal * (overlap / 2.0f);
                bodyB.position -= collisionNormal * (overlap / 2.0f);

                // Reverse velocities along the collision normal
                glm::vec3 relativeVelocity = bodyB.velocity - bodyA.velocity;
                float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

                if (velocityAlongNormal < 0.0f) {
                    float restitution = 0.8f; // Elasticity coefficient
                    float impulse = -(1 + restitution) * velocityAlongNormal;
                    impulse /= (1 / bodyA.mass) + (1 / bodyB.mass);

                    glm::vec3 impulseVector = impulse * collisionNormal;
                    bodyA.velocity -= impulseVector / bodyA.mass;
                    bodyB.velocity += impulseVector / bodyB.mass;
                }
            }
        }
    }
}
