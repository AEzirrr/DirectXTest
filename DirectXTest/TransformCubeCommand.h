#pragma once
#include "Command.h"
#include "Cube.h"
#include "Vector3D.h"

class TransformCubeCommand : public Command {
public:
    // We pass the cube, and the NEW target values for its transform
    TransformCubeCommand(Cube* cube, Vector3D newPos, Vector3D newRot, Vector3D newScale)
        : m_cube(cube), m_newPos(newPos), m_newRot(newRot), m_newScale(newScale)
    {
        // 1. Take a snapshot of the OLD state before we change anything
        // (You may need to add simple getPosition(), getRotation(), getScale() to your Cube class)
        m_oldPos = m_cube->getLocalPosition();
        m_oldRot = m_cube->getLocalRotation();
        m_oldScale = m_cube->getLocalScale();
    }

    void execute() override {
        // Apply the NEW state
        m_cube->setPosition(m_newPos.x, m_newPos.y, m_newPos.z);
        m_cube->setRotation(m_newRot.x, m_newRot.y, m_newRot.z);
        m_cube->setScale(m_newScale.x, m_newScale.y, m_newScale.z);
    }

    void undo() override {
        // Revert to the OLD snapshot
        m_cube->setPosition(m_oldPos.x, m_oldPos.y, m_oldPos.z);
        m_cube->setRotation(m_oldRot.x, m_oldRot.y, m_oldRot.z);
        m_cube->setScale(m_oldScale.x, m_oldScale.y, m_oldScale.z);
    }

private:
    Cube* m_cube;

    // The Snapshot (Old State)
    Vector3D m_oldPos;
    Vector3D m_oldRot;
    Vector3D m_oldScale;

    // The Target (New State)
    Vector3D m_newPos;
    Vector3D m_newRot;
    Vector3D m_newScale;
};