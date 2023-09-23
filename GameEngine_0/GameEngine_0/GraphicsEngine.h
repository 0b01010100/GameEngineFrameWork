#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;


class GraphicsEngine
{
public:
	GraphicsEngine();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~GraphicsEngine();
public:
	//PURE FUNCTIONS
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	//
	//Creates a Vertex Shader 
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);
public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
public:
	//Returns a pointer to the Graphics Engine Instance
	static GraphicsEngine* get();

private:
	DeviceContext* m_imm_device_context = nullptr;
private:
	//Points to the Device that was created in the D3D11CreateDevice Function
	//The ID3D11Device is Grahics Hardware that provides an interface for creating 
	//and Manging resources, rendering, and other Grahics related task.
	ID3D11Device* m_d3d_device = nullptr;
	//Holds the Feature level of DirectX that was Chosen
	D3D_FEATURE_LEVEL m_feature_level;
private:
	//DXGI is used for managing graphics adapters, outputs, and presenting rendered frames.
	IDXGIDevice* m_dxgi_device = nullptr;
	//Graphics card adapter. This interface can be useful for tasks like enumerating available adapters, querying information about a specific adapter, and managing display outputs in a DirectX application.
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	//The DXGI factory is responsible for creating and managing resources related to graphics adapters, such as swap chains, rendering targets, and more. 
	IDXGIFactory* m_dxgi_factory = nullptr;
	ID3D11DeviceContext* m_imm_context = nullptr;

private:
	ID3DBlob* m_blob = nullptr;
	
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
private:
	//These classes a Friends becuase they have private Properties that have to be accesed by the class.
	friend class SwapChain; 
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};