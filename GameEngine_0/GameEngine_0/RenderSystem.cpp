#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem()
{
}

bool RenderSystem::init()
{
	//Drivers for the Grahics Devive AKA: Graphic card which will help provide good rendering performance for most computers
	D3D_DRIVER_TYPE driver_types[] =
	{
		//Drawing is mainly Executed to the GPU when using this Driver
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

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		//This Functions allows us the Create a Divece which will allow use the draw things on the Screen.
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		//Loops throw the Prossece of trying to Create a device unitl the Device is Created 
		if (SUCCEEDED(res))//Checks to See if the Prossece of creating the device SUCCEEDED
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context, this);

	//The purpose of this code is to obtain the DXGI(DirectX Graphics Infrastructure) device interface from the existing Direct3D device.
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	//Helps gather informations about the Grahpics Adapter
	////m_dxgi_device->GetAdapter(&m_dxgi_adapter);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	//Helps use obtain the IDXGIFactory interface, which is responsible for creating and managing resources related to graphics adapters, such as swap chains, rendering targets, and more. 
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}

//Realses the Resources 
bool RenderSystem::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	delete m_imm_device_context;


	m_d3d_device->Release();
	return true;
}

RenderSystem::~RenderSystem()
{
}
//Creates a Swap Chain so we can do Double Buffering 
SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* sc = nullptr;

	try
	{
		sc = new SwapChain(this, hwnd, width, height);
	}
	catch (...)
	{

	}
	return sc;
}

//Return the Current DeviceContex Variable 
DeviceContext* RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}
//Creates a Vetex Shader
VertexBuffer* RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
	void* shader_byte_code, size_t size_byte_shader)
{
	VertexBuffer* vb = nullptr;
	try
	{
		vb = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch (const std::exception&)
	{

	}
	return vb;  
}

IndexBuffer* RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* ib = nullptr;
	try
	{
		ib = new IndexBuffer(list_indices, size_list ,this);
	}
	catch (const std::exception&)
	{

	}

	return ib;
}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* cb = nullptr;
	try
	{
		cb = new ConstantBuffer(buffer, size_buffer ,this);
	}
	catch (const std::exception&)
	{
		
	}
	return cb;
}

VertexShader* RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = nullptr;
	try
	{
		vs = new VertexShader(shader_byte_code, byte_code_size, this);
	}
	catch (const std::exception&)
	{

	}
	return vs;
}

PixelShader* RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = nullptr;
	try
	{
		ps = new PixelShader(shader_byte_code, byte_code_size, this);
	}
	catch (const std::exception&)
	{

	}
	return ps;
}
/*Compiles the Instructions on how to make a Vextex shader from HLSL to Binary so the code can be read*/
bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	//Hold Messages related to Erros in the Compilelations
	ID3DBlob* error_blob = nullptr;
	//Cancles the COmpilations Process if it Failed
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}
//Compile the Instructions for making a Pixel shader From HLSL to Binary, so code can be read.
bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();
}

