#include "SceneCameraHandler.h"
#include "InputSystem.h"

SceneCameraHandler* SceneCameraHandler::sharedInstance = nullptr;

SceneCameraHandler* SceneCameraHandler::getInstance()
{
	return sharedInstance;
}

void SceneCameraHandler::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new SceneCameraHandler();
	}
}

void SceneCameraHandler::destroy()
{
	if (sharedInstance != nullptr) {
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

SceneCameraHandler::SceneCameraHandler()
{
	sceneCamera = new Camera("SceneCamera");

	InputSystem::getInstance()->addListener(sceneCamera);
}

SceneCameraHandler::~SceneCameraHandler()
{
	InputSystem::getInstance()->removeListener(sceneCamera);
	delete sceneCamera;
}

void SceneCameraHandler::update(float deltaTime)
{
	if (sceneCamera != nullptr) {
		sceneCamera->update(deltaTime);
	}
}

Matrix4x4 SceneCameraHandler::getSceneCameraViewMatrix()
{
	if (sceneCamera != nullptr) {
		return sceneCamera->getViewMatrix();
	}

	Matrix4x4 empty;
	empty.setIdentity();
	return empty;
}

Camera* SceneCameraHandler::getCamera()
{
	return sceneCamera;
}