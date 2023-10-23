#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>
//This will do all of the set up needed for the Swap Chain
SwapChain::SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;
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

	//create swap chain for window indicated by HWND parameter
	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
	//If the Process fails then return false 
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not Created successfully");
	}
	//Get the back buffer color and create its render target view
	ID3D11Texture2D* buffer = NULL;
	//The Methode will retrive the buffer in the Computer monitor.
	//All buffers do is work together to present things on the screen.
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	//Checks to see if the Process failed or not, if it did we will exit this function returning false
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not Created successfully");
	}
	//Allows us to paint on to the RenderTargetView, with what can be called a Buffer.
	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	//To Stop painting on the buffer
	buffer->Release();

	if (FAILED(hr))
	{
		throw std::exception("SwapChain not Created successfully");
	}
}


//Tells the swap chain to present the texture on the screen.
bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}
//Realses the Swap Chain instance
SwapChain::~SwapChain()
{
	m_rtv->Release();
	m_swap_chain->Release();
}