#include "SwapChain.h"
#include "GraphicsEngine.h"
SwapChain::SwapChain()
{
}
//This will do all of the set up needed for the Swap Chain
bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));//Allows use to fill the memory being used with zero
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width; // width of the Window
	desc.BufferDesc.Height = height; //height of the Window
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//Dettermins how much color should be in Every pixel on the Screen.
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
	if (FAILED(hr))
	{
		return false;
	}
	//The buffer Pointer
	ID3D11Texture2D* buffer = NULL;
	//The Methode will retrive the buffer in the Computer monitor.
	//All buffers do is work together to present things on the screen.
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	//Checks to see if the Process failed or not, if it did we will exit this function returning false
	if (FAILED(hr))
	{
		return false;
	}
	//Allows us to paint on to the Texture, with what can be called a Buffer.
	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	//To Stop painting on the buffer
	buffer->Release();

	if (FAILED(hr))
	{
		return false;
	}


	return true;
}
//Tells the swap chain to present the texture on the screen.
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