#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	//Drivers for the Grahics Devive AKA: Graphic card which will help provide good rendering performance for most computers
	D3D_DRIVER_TYPE driver_types[] =
	{
		//Drawing is mainly Exucuted to the GPU when using this Driver
		D3D_DRIVER_TYPE_HARDWARE,
		//This is a very fast Driver that has the CPU to do most of the Math to get thing draw on the screen.
		D3D_DRIVER_TYPE_WARP,
		//Compatbale with most DirectX Features, but is pretty slow compared the the First two Drivers.
		D3D_DRIVER_TYPE_REFERENCE
	};
	//Gets the amount of objects int the Array and assigns them to a integral variable 
	//called num_driver_types
	UINT num_driver_types = ARRAYSIZE(driver_types);
	//The rendering feature from DirectX to target 
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	//Gets the amount of objects int the Array and assigns them to a integral variable 
	//called num_feature_levels
	UINT num_feature_levels = ARRAYSIZE(feature_levels);
	//An integral Variable which will somehow make sure that The
	//Variables below are handled corretly before exiting the function
	HRESULT res = 0;
	ID3D11DeviceContext* m_imm_context = nullptr;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		//This Functions allows us the Create a Divece which will allow use the draw things on the Screen.
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		//Loops throw the Prossece of trying to Create a device unitl the Device is Created 
		if (SUCCEEDED(res))//Checks to See if the Prossece of creating the device SUCCEEDED
			break;
		++driver_type_index;//index number advances
	}
	if (FAILED(res))
	{
		return false;
	}
	m_imm_device_context = new DeviceContext(m_imm_context);
	//The purpose of this code is to obtain the DXGI(DirectX Graphics Infrastructure) device interface from the existing Direct3D device.
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	//Helps gather informations about the Grahpics Adapter
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	//Helps use obtain the IDXGIFactory interface, which is responsible for creating and managing resources related to graphics adapters, such as swap chains, rendering targets, and more. 
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
		
	return true;
}


bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	//m_imm_context->Release();

	m_d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}