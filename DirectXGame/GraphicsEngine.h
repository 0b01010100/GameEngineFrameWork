#pragma once
#include <d3d11.h>
//#include "SwapChain.h"

class SwapChain;
class DeviceContext;
class GraphicsEngine
{
public:
	GraphicsEngine();
	//initialize grahpicsengine and DirectX 11 Device
	bool init();
	//releases the same
	bool release();
	~GraphicsEngine();
public:
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
public:
	static GraphicsEngine* get();
private:
	DeviceContext* m_imm_device_context;
private:
	ID3D11Device* m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
private:
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
private:
	friend class SwapChain;
};

