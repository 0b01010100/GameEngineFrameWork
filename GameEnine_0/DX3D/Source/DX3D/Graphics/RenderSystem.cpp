#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>

#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem()
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
	ui32 num_driver_types = ARRAYSIZE(driver_types);
	//The rendering feature from DirectX to target 
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	//Gets the amount of objects int the Array and assigns them to a integral variable 
	//called num_feature_levels
	ui32 num_feature_levels = ARRAYSIZE(feature_levels);
	//An integral Variable which will somehow make sure that The
	//Variables below are handled corretly before exiting the function
	HRESULT res = 0;

	D3D_FEATURE_LEVEL featureLevel = {};

	for (ui32 driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		//This Functions allows us the Create a Divece which will allow use the draw things on the Screen.
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &featureLevel, &m_imm_context);
		//Loops throw the Prossece of trying to Create a device unitl the Device is Created 
		if (SUCCEEDED(res))//Checks to See if the Prossece of creating the device SUCCEEDED
			break;
		++driver_type_index; 

	}
	if (FAILED(res))
	{
		DX3DError("RenderSystem not Created successfully");
	}

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context.Get(), this );

	//The purpose of this code is to obtain the DXGI(DirectX Graphics Infrastructure) device interface from the existing Direct3D device.
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	//Helps gather informations about the Grahpics Adapter
	////m_dxgi_device->GetAdapter(&m_dxgi_adapter);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	//Helps use obtain the IDXGIFactory interface, which is responsible for creating and managing resources related to graphics adapters, such as swap chains, rendering targets, and more. 
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	initRasterizerStates();
	compilePrivateShaders ( );
}

RenderSystem::~RenderSystem ( )
{
}

//Creates a Swap Chain so we can do Double Buffering 
SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, ui32 width, ui32 height)
{
	return std::make_shared<SwapChain>(hwnd, width, height, this);
}

//Return the Current DeviceContex Variable 
DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}
//Creates a Vetex Shader
VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list)
{
	return std::make_shared< VertexBuffer>(list_vertices, size_vertex, size_list, this);
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, ui32 size_list)
{
	return std::make_shared< IndexBuffer>(list_indices, size_list ,this);
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, ui32 size_buffer)
{
	return std::make_shared<ConstantBuffer>(buffer, size_buffer ,this);
}

VertexShaderPtr RenderSystem::createVertexShader( const wchar_t* full_path, const char* entryPoint )
{
	return  std::make_shared<VertexShader>( full_path, entryPoint, this);
}

PixelShaderPtr RenderSystem::createPixelShader( const wchar_t* full_path, const char* entryPoint )
{
	return  std::make_shared<PixelShader> ( full_path, entryPoint, this );
}
Texture2DPtr RenderSystem::createTexture ( const wchar_t* full_path )
{
	return std::make_shared<Texture2D> ( full_path, this );
}
Texture2DPtr RenderSystem::createTexture ( const Rect& size, Texture2D::Type type )
{
	return std::make_shared<Texture2D> ( size, type, this );
}
///*Compiles the Instructions on how to make a Vextex shader from HLSL to Binary so the code can be read*/
//bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
//{
//	//Hold Messages related to Erros in the Compilelations
//	ID3DBlob* error_blob = nullptr;
//	//Cancles the COmpilations Process if it Failed
//	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
//	{
//		if (error_blob) error_blob->Release();
//		return false;
//	}
//	
//	*shader_byte_code = m_blob->GetBufferPointer();
//	*byte_code_size = m_blob->GetBufferSize();
//
//	return true;
//}
////Compile the Instructions for making a Pixel shader From HLSL to Binary, so code can be read.
//bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
//{
//	ID3DBlob* error_blob = nullptr;
//	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
//	{
//		if (error_blob) error_blob->Release();
//		return false;
//	}
//
//	*shader_byte_code = m_blob->GetBufferPointer();
//	*byte_code_size = m_blob->GetBufferSize();
//
//	return true;
//}
//
//void RenderSystem::releaseCompiledShader()
//{
//	if (m_blob)m_blob->Release();
//}

void RenderSystem::setCullMode(const CullMode& mode)
{
	if (mode == CullMode::Front)
	{
		//Cull the front face of an object when rasterizing it
		m_imm_context->RSSetState(m_cull_front_state.Get());
	}
	else if (mode == CullMode::Back)
		//Cull the back face of an object when rasterizing it
		m_imm_context->RSSetState(m_cull_back_state.Get());
	else if (mode == CullMode::None) 
	{
		m_imm_context->RSSetState ( m_cull_none_state.Get ( ) );
	}

}

void RenderSystem::compilePrivateShaders ( )
{
	//Hold Messages related to Erros in the Compilelations
	Microsoft::WRL::ComPtr<ID3DBlob> blob;//Stores the compiled shader code
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;//Stores errors while compiling the shader the code, if there are any.

	//This haders diffens how data show be interrpreted by the vertex and shaders.
	auto meshLayoutCode = R"(
struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	return output;
}
	)";
	//Gets the Length of the code 
	auto codeSize = strlen ( meshLayoutCode );


	//Compiles the shader then checks if the Process Faild.
	if (FAILED ( D3DCompile ( meshLayoutCode, codeSize, "VertexMeshLayoutShader"/*Shader Name*/, nullptr, nullptr, "main",
		"vs_5_0", 0, 0, &blob, &errorBlob ) ))
	{
		DX3DError ( "VertexMeshLayoutShader not compiled successfully" );
	}
	memcpy ( m_meshLayoutByteCode, blob->GetBufferPointer ( ), blob->GetBufferSize ( ) );
	m_meshLayoutSize = blob->GetBufferSize ( );
}

void RenderSystem::initRasterizerStates()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;

	desc.CullMode = D3D11_CULL_FRONT;
	m_d3d_device->CreateRasterizerState( &desc, &m_cull_front_state);

	desc.CullMode = D3D11_CULL_BACK;
	m_d3d_device->CreateRasterizerState( &desc, &m_cull_back_state);

	desc.CullMode = D3D11_CULL_NONE;
	m_d3d_device->CreateRasterizerState ( &desc, &m_cull_none_state );
}

