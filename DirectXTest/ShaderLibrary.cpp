#include "ShaderLibrary.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"

ShaderLibrary* ShaderLibrary::sharedInstance = nullptr;

ShaderLibrary::ShaderLibrary()
{
}

ShaderLibrary::~ShaderLibrary()
{
	for (auto& pair : activeVertexShaders)
	{
		if (pair.second) pair.second->release();
	}
	activeVertexShaders.clear();

	for (auto& pair : activePixelShaders)
	{
		if (pair.second) pair.second->release();
	}
	activePixelShaders.clear();
}

ShaderLibrary* ShaderLibrary::getInstance()
{
	return sharedInstance;
}

void ShaderLibrary::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new ShaderLibrary();
	}
}

void ShaderLibrary::destroy()
{
	if (sharedInstance)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

// Compiles a vertex shader fresh and hands back a PERMANENT copy of its byte code.
// Primitives need this raw byte code (not the VertexShader object) to build their
// input layout when creating a vertex buffer.
void ShaderLibrary::requestVertexShaderData(String vertexShaderName, void** shaderByteCode, size_t* sizeShader)
{
	void* temp_byte_code = nullptr;
	size_t temp_size = 0;

	GraphicsEngine::getInstance()->compileVertexShader(vertexShaderName.c_str(), "vsmain", &temp_byte_code, &temp_size);

	// make a permanent copy — the compiler's temp buffer gets released elsewhere
	*sizeShader = temp_size;
	*shaderByteCode = new unsigned char[temp_size];
	memcpy(*shaderByteCode, temp_byte_code, temp_size);

	GraphicsEngine::getInstance()->releaseCompiledShader();
}

VertexShader* ShaderLibrary::getVertexShader(String vertexShaderName)
{
	auto it = activeVertexShaders.find(vertexShaderName);
	if (it != activeVertexShaders.end())
	{
		return it->second;
	}

	// not compiled yet — compile, create, and cache it
	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;
	requestVertexShaderData(vertexShaderName, &shaderByteCode, &sizeShader);

	VertexShader* vertexShader = GraphicsEngine::getInstance()->createVertexShader(shaderByteCode, sizeShader);

	delete[](unsigned char*)shaderByteCode;

	activeVertexShaders[vertexShaderName] = vertexShader;
	return vertexShader;
}

PixelShader* ShaderLibrary::getPixelShader(String pixelShaderName)
{
	auto it = activePixelShaders.find(pixelShaderName);
	if (it != activePixelShaders.end())
	{
		return it->second;
	}

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::getInstance()->compilePixelShader(pixelShaderName.c_str(), "psmain", &shaderByteCode, &sizeShader);
	PixelShader* pixelShader = GraphicsEngine::getInstance()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

	activePixelShaders[pixelShaderName] = pixelShader;
	return pixelShader;
}