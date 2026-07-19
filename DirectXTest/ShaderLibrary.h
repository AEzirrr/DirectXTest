#pragma once
#include <string>
#include <unordered_map>

class VertexShader;
class PixelShader;

class ShaderLibrary
{
public:
	class ShaderData {
	public:
		void* shaderByteCode = nullptr;
		size_t sizeShader = 0;
	};

	typedef std::wstring String;
	typedef std::unordered_map<String, VertexShader*> VertexShaderTable;
	typedef std::unordered_map<String, PixelShader*> PixelShaderTable;

	static ShaderLibrary* getInstance();
	static void initialize();
	static void destroy();

	// since shader byte code gets dereferenced for some reason if stored in a hash table, request manually.
	void requestVertexShaderData(String vertexShaderName, void** shaderByteCode, size_t* sizeShader);

	VertexShader* getVertexShader(String vertexShaderName);
	PixelShader* getPixelShader(String pixelShaderName);

private:
	ShaderLibrary();
	~ShaderLibrary();
	ShaderLibrary(ShaderLibrary const&) {};              // copy constructor is private
	ShaderLibrary& operator=(ShaderLibrary const&) { return *this; }; // assignment operator is private
	static ShaderLibrary* sharedInstance;

	VertexShaderTable activeVertexShaders;
	PixelShaderTable activePixelShaders;
};