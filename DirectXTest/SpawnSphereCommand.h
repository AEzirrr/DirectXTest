// SpawnCubeCommand.h
#pragma once
#include "Command.h"
#include "Sphere.h"
#include "Vector3D.h"
#include <vector>
#include <algorithm>

class SpawnSphereCommand : public Command {
public:
    SpawnSphereCommand(std::vector<Sphere*>& sphereList, void* shader_byte_code, size_t size_shader, Vector3D spawnPos)
        : m_sphereList(sphereList), m_shader_byte_code(shader_byte_code), m_size_shader(size_shader), m_spawnPos(spawnPos), m_sphere(nullptr), m_isActive(false) {}

    ~SpawnSphereCommand() {

        if (m_sphere && !m_isActive) {
            delete m_sphere;
        }
    }

    void execute() override {
        if (!m_sphere) {
			// Sphere intialization
            m_sphere = new Sphere("SpawnedSphere", m_shader_byte_code, m_size_shader);
            m_sphere->setPosition(m_spawnPos.x, m_spawnPos.y, m_spawnPos.z);
            m_sphere->setScale(0.5f, 0.5f, 0.5f);

            // random direction on spawn
            float randX = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            float randY = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            float randZ = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

            m_sphere->setDirection(Vector3D(randX, randY, randZ));
        }

        m_sphereList.push_back(m_sphere); // Add to the render list
        m_isActive = true;
    }

    void undo() override {
        auto it = std::find(m_sphereList.begin(), m_sphereList.end(), m_sphere);
        if (it != m_sphereList.end()) {
            m_sphereList.erase(it);
        }
        m_isActive = false;
    }

private:
    std::vector<Sphere*>& m_sphereList;
    void* m_shader_byte_code;
    size_t m_size_shader;
    Vector3D m_spawnPos;

    Sphere* m_sphere;
    bool m_isActive;
};