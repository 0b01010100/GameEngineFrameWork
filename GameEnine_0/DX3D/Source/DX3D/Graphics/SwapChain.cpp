#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <exception>
//This will do all of the set up needed for the Swap Chain
SwapChain::SwapChain(HWND hwnd, ui32 width, ui32 height, RenderSystem* system) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device.Get();
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));//Allows use to fill the memory being used with zero
	desc.BufferCount = 1;//AMount of back buffers
	desc.BufferDesc.Width = width; // width of the Window
	desc.BufferDesc.Height = height; //height of the Window
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //This part of the format specifies the color channels for each pixel.
	desc.BufferDesc.RefreshRate.Numerator = 60;// Refresh rate
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;//Window to present on
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;//The mode of the Window for example is the Window full screen or not.
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//
	//create swap chain for window indicated by HWND parameter
	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
	//If the Process fails then return false 
	if (FAILED(hr))
	{
		DX3DError("SwapChain not Created successfully");
	}
	reloadBuffers(width, height);

}


void SwapChain::setFullScreen(bool fullscreen, ui32 width, ui32 height)
{
	resize(width, height);
	m_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::resize(ui32 width, ui32 height)
{
	m_rtv.Reset ( );
	m_dsv.Reset ( );

	m_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	reloadBuffers(width , height);
}

//Tells the swap chain to present the texture on the screen.
bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

void SwapChain::reloadBuffers(ui32 width, ui32 height)
{
	ID3D11Device* device = m_system->m_d3d_device.Get();
	HRESULT hr;
	//Get the back buffer color and create its render target view
	ID3D11Texture2D* buffer = NULL;
	//The Methode will retrive the buffer in the Computer monitor.
	//All buffers do is work together to present things on the screen.
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	//Checks to see if the Process failed or not, if it did we will exit this function returning false
	if (FAILED(hr))
	{
		DX3DError("SwapChain not Created successfully");
	}
	//Allows us to paint on to the RenderTargetView, with what can be called a Buffer.
	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	//To Stop painting on the buffer
	buffer->Release();

	if (FAILED(hr))
	{
		DX3DError("SwapChain not Created successfully");
	}
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = (width <= 0) ? 1 : width;
	tex_desc.Height = (height <= 0) ? 1 : height;//Stops the height from being negative or 0
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;// Depth value type unsigned normalized integer; Stencil vaule type: unsigned integer
	tex_desc.Usage = D3D11_USAGE_DEFAULT;//GPU can read and write to the resource and not directly modifyable by the CPU
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//Since texture sever multiple purposes  we have to tell the computer what is the purpose of he texture. This turture will store depth data so parts of an obj can overlap another
	tex_desc.MipLevels = 1;// Amount of precomputed, smaller versions of a original texture. These textures have a dilation of 1/2  of the previous texture
	tex_desc.SampleDesc.Count = 1;//How much to sample
	tex_desc.SampleDesc.Quality = 0;//The Quality of the sample
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;//The CPU has no access rights

	device->CreateTexture2D(&tex_desc, nullptr, &buffer);


	if (FAILED(hr))
	{
		DX3DError("SwapChain not Created successfully");
	}


	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	//To Stop painting on the buffer
	buffer->Release();

	if (FAILED(hr))
	{
		DX3DError("SwapChain not Created successfully");
	}

}
