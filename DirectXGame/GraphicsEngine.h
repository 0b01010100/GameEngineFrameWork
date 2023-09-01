#pragma once
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include <d3dcompiler.h>
#include <d3d11.h>//Importing this allows use to Access the DircetX API
#include "SwapChain.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class GraphicsEngine
{
public:
	GraphicsEngine();
	//initialize grahpicsengine and DirectX 11 Device
	bool init();
	//This Function should be called to realse all of the compuers resoures used after before the program is shut down. 
	//All Most like reycling, but the Computer memory  would be Recycled
	bool release();
	~GraphicsEngine();
public:
	//Creates a Swap chain, which is a collections of Frame Buffers that help render frames on screen
	SwapChain* createSwapChain();
	//Returns a Pointer to the DeviceContext
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
public:
	bool createShaders();
	bool setShaders();
	void getShaderBufferAndSize(void** bytecode, UINT* size);
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

	ID3D11DeviceContext* m_imm_context;
private:
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
private:
	friend class SwapChain; // Adding Swap Chain as a friend class to access the m_swap_chain member in the SwapChain Class.
	friend class VertexBuffer; // Adding VertexBuffer as a friend class to access the m_swap_chain member in the SwapChain Class.
	
};

