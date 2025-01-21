#pragma once

#include <vector>
#include <glm/glm.hpp>

struct RigidBody {
    glm::vec3 position;     // Object position
    glm::vec3 velocity;     // Current velocity
    glm::vec3 acceleration; // Acceleration (e.g., gravity)
    float mass;             // Mass of the object
    float radius;           // Radius for collision detection (assuming a spherical shape)

    RigidBody(glm::vec3 pos, glm::vec3 vel, float m, float r)
        : position(pos), velocity(vel), acceleration(0.0f), mass(m), radius(r) {}
};

class PhysicsEngine {
public:
    void addObject(const RigidBody& body); // Add a new rigid body
    void update(float deltaTime);          // Update all physics objects
    void detectAndResolveCollisions();     // Detect and resolve collisions

    const std::vector<RigidBody>& getObjects() const { return objects; }

private:
    std::vector<RigidBody> objects;        // List of all physics objects
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f); // Gravity force
};
