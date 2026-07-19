#pragma once
#include <string>

class ShaderNames
{
public:
	typedef std::wstring String;

	//vertex shader names
	const String BASE_VERTEX_SHADER_NAME = L"VertexShader.hlsl";
	const String TEXTURED_VERTEX_SHADER_NAME = L"TexturedVertexShader.hlsl";

	//pixel shader names
	const String BASE_PIXEL_SHADER_NAME = L"PixelShader.hlsl";
	const String TEXTURED_PIXEL_SHADER_NAME = L"TexturedPixelShader.hlsl";
};