#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "SpawnSphereCommand.h"
#include "TransformCubeCommand.h"
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "SceneCameraHandler.h"
#include "Material.h"
//ImGui includes and screens
#include "UIManager.h"
#include "InspectorScreen.h"
#include "HierarchyScreen.h"
#include "ToolbarScreen.h"
#include "ProfilerScreen.h"
#include "MaterialInspectorScreen.h"
#include "ShaderLibrary.h"
#include "ShaderNames.h"

__declspec(align(16)) struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	unsigned int m_time;
	Vector3D m_color;
};

struct vertex {
	Vector3D position;
	Vector3D color;
};

AppWindow::AppWindow()
{}

AppWindow::~AppWindow()
{}

void AppWindow::update()
{}

void AppWindow::onCreate()
{
	SceneCameraHandler::initialize();
	InputSystem::getInstance()->addListener(this);
	InputSystem::getInstance()->showCursor(true);

	GraphicsEngine::getInstance()->init();
	m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();

	UIManager::getInstance()->initialize(this->m_hwnd);


	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	rasterizerState = new RasterizerState();
	rasterizerState->InitializeWireframe(GraphicsEngine::getInstance()->getDevice());

	// --- Inspector Setup ---
	AUIScreen* inspector = UIManager::getInstance()->getScreen("Inspector");

	// --- Hierarchy Setup ---
	AUIScreen* hierarchy = UIManager::getInstance()->getScreen("Hierarchy");

	// --- Toolbar Setup ---
	AUIScreen* toolbar = UIManager::getInstance()->getScreen("Toolbar");
	if (toolbar != nullptr) {
		ToolbarScreen* customToolbar = static_cast<ToolbarScreen*>(toolbar);
		customToolbar->setListener(this);
	}

	// --- Profiler Setup ---
	AUIScreen* profiler = UIManager::getInstance()->getScreen("Profiler");

	// --- Material Inspector Setup ---
	AUIScreen* materialInspector = UIManager::getInstance()->getScreen("Material Inspector");


	m_old_time = std::chrono::steady_clock::now(); // intialize th old time

	ShaderLibrary::initialize();

	ShaderNames shaderNames;
	ShaderLibrary::getInstance()->requestVertexShaderData(
		shaderNames.BASE_VERTEX_SHADER_NAME, &m_shader_byte_code, &m_size_shader);

	constant cc;
	cc.m_view = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cc.m_projection = m_projection_matrix;
	cc.m_time = 0;

	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	InputSystem::getInstance()->update();
	CommandSystem::getInstance()->processQueue();
	constant cc;


	// IMPROVED FRAME TIMING
	auto current_time = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = current_time - m_old_time;

	deltaTime = elapsed.count();

	m_old_time = current_time;

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(width, height);

	SceneCameraHandler::getInstance()->update(deltaTime);
	cc.m_view = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cc.m_projection.setPerspectiveFovLH(1.04f, (float)width / (float)height, 0.5f, 1000.0f);

	for (int i = 0; i < m_game_objects.size(); i++)
	{
		m_game_objects[i]->update(deltaTime);
		m_game_objects[i]->draw(cc.m_projection);
	}

	UIManager::getInstance()->drawAllUI();

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	if (m_shader_byte_code) {
		delete[](unsigned char*)m_shader_byte_code;
		m_shader_byte_code = nullptr;
	}

	rasterizerState->release();
	m_swap_chain->release();

	SceneCameraHandler::destroy();

	if (m_vertex_buffer) m_vertex_buffer->release();
	if (m_index_buffer) m_index_buffer->release();

	if (m_game_objects.size() > 0) {
		for (int i = 0; i < m_game_objects.size(); i++) {
			if (m_game_objects[i]) {
				delete m_game_objects[i];
				m_game_objects[i] = nullptr;
			}
		}
		m_game_objects.clear();
	}

	UIManager::getInstance()->destroy();

	ShaderLibrary::destroy();

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->release();
}

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{

	if (key == 'O') {
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->setRasterizerState(rasterizerState);
	}
	if (key == 'P') {
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRasterizerState(rasterizerState);
	}

	if (key == VK_ESCAPE) { // Exit the application when escape is pressed
		this->m_is_running = false;
	}
}

void AppWindow::onKeyUp(int key)
{

	if (key == VK_SPACE) {

	}

	if (key == VK_BACK) {
		CommandSystem::getInstance()->undo(); // Remove the recently spawned sphere command using undo 
		
	}

	if (key == VK_DELETE) {

	}
	

	bool isCtrlHeld = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	if (isCtrlHeld && key == 'Z') { // Undo last command
		CommandSystem::getInstance()->undo();
	}

	if (isCtrlHeld && key == 'Y') { // Redo last command
		CommandSystem::getInstance()->redo();
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{

}

void AppWindow::onMouseWheel(int delta)
{

}

void AppWindow::onCreateCubeClicked()
{
	int counter = m_game_objects.size() + 1;
	std::string cubeName = "Cube_" + std::to_string(counter);

	bool nameExists = true;
	while (nameExists)
	{
		nameExists = false;
		for (const auto& obj : m_game_objects)
		{
			// Adjust "getName()" to your actual property/method for the object's name
			if (obj->getName() == cubeName)
			{
				nameExists = true;
				counter++;
				cubeName = "Cube_" + std::to_string(counter);
				break;
			}
		}
	}

	Cube* newCube = new Cube(	cubeName, m_shader_byte_code, m_size_shader);
	newCube->setPosition(0.0f, 0.0f, 0.0f);
	newCube->setScale(0.5f, 0.5f, 0.5f);

	m_game_objects.push_back(newCube);

	AUIScreen* hierarchy = UIManager::getInstance()->getScreen("Hierarchy");
	if (hierarchy != nullptr) {
		HierarchyScreen* customHierarchy = static_cast<HierarchyScreen*>(hierarchy);
		customHierarchy->addObject(newCube);
	}
}

void AppWindow::onCreateSphereClicked()
{
	int counter = m_game_objects.size() + 1;
	std::string sphereName = "Sphere_" + std::to_string(counter);

	bool nameExists = true;
	while (nameExists)
	{
		nameExists = false;
		for (const auto& obj : m_game_objects)
		{
			if (obj->getName() == sphereName)
			{
				nameExists = true;
				counter++;
				sphereName = "Sphere_" + std::to_string(counter);
				break;
			}
		}
	}

	Sphere* newSphere = new Sphere(sphereName, m_shader_byte_code, m_size_shader);
	newSphere->setPosition(0.0f, 0.0f, 0.0f);
	newSphere->setScale(1.0f, 1.0f, 1.0f);
	m_game_objects.push_back(newSphere);
	AUIScreen* hierarchy = UIManager::getInstance()->getScreen("Hierarchy");
	if (hierarchy != nullptr) {
		HierarchyScreen* customHierarchy = static_cast<HierarchyScreen*>(hierarchy);
		customHierarchy->addObject(newSphere);
	}
}

void AppWindow::onCreatePlaneClicked()
{
	int counter = m_game_objects.size() + 1;
	std::string planeName = "Plane_" + std::to_string(counter);

	bool nameExists = true;
	while (nameExists)
	{
		nameExists = false;
		for (const auto& obj : m_game_objects)
		{
			if (obj->getName() == planeName)
			{
				nameExists = true;
				counter++;
				planeName = "Plane_" + std::to_string(counter);
				break;
			}
		}
	}

	Plane* newPlane = new Plane(planeName, m_shader_byte_code, m_size_shader);
	newPlane->setPosition(0.0f, 0.0f, 0.0f);
	newPlane->setScale(2.0f, 1.0f, 2.0f);

	m_game_objects.push_back(newPlane);

	AUIScreen* hierarchy = UIManager::getInstance()->getScreen("Hierarchy");
	if (hierarchy != nullptr) {
		HierarchyScreen* customHierarchy = static_cast<HierarchyScreen*>(hierarchy);
		customHierarchy->addObject(newPlane);
	}
}

void AppWindow::onCreateCylinderClicked()
{
	int counter = m_game_objects.size() + 1;
	std::string cylinderName = "Cylinder_" + std::to_string(counter);

	bool nameExists = true;
	while (nameExists)
	{
		nameExists = false;
		for (const auto& obj : m_game_objects)
		{
			if (obj->getName() == cylinderName)
			{
				nameExists = true;
				counter++;
				cylinderName = "Cylinder_" + std::to_string(counter);
				break;
			}
		}
	}

	Cylinder* newCylinder = new Cylinder(cylinderName, m_shader_byte_code, m_size_shader);
	newCylinder->setPosition(0.0f, 0.0f, 0.0f);
	newCylinder->setScale(1.0f, 1.0f, 1.0f);

	m_game_objects.push_back(newCylinder);

	AUIScreen* hierarchy = UIManager::getInstance()->getScreen("Hierarchy");
	if (hierarchy != nullptr) {
		HierarchyScreen* customHierarchy = static_cast<HierarchyScreen*>(hierarchy);
		customHierarchy->addObject(newCylinder);
	}
}