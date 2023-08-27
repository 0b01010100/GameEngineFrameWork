#include "SwapChain.h"
#include "GraphicsEngine.h"
SwapChain::SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));//Allows use to fill the memory being used with zero
	desc.BufferCount =2; 
	desc.BufferDesc.Width = width; // width of the Window
	desc.BufferDesc.Height = height; //height of the Window
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//Dettermins how much color should be in Evey pixel on the Screen.
	desc.BufferDesc.RefreshRate.Numerator = 60;// Refresh rate
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;//The mode of the Window for example is the Window full screen or not.

	//create swap chain for window indicated by HWND parameter
	HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
	//If the Process fails then return false 
	if(FAILED(hr))
	{
		return false;
	}

	ID3D11Texture2D* buffer = NULL;
	hr=m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if(FAILED(hr))
	{
		return false;
	}

	hr=device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))
	{
		return false;
	}


	return true;
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}
//Realses the Swap Chain instance
bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;
	return true;
}

SwapChain::~SwapChain()
{
}
