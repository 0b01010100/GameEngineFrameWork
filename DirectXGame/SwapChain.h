#pragma once
#include <d3d11.h>


class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	//This Will Help with setting up the Buffers for rendering things on the screen
	bool init(HWND hwnd, UINT width, UINT height );


	bool present(bool vsync);

	bool release();
	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain = NULL;
	//Specfies a target surface or buffer where rendering operations will be performed. 
	ID3D11RenderTargetView* m_rtv = NULL;
	ID3D11DepthStencilView* m_dsv;
private:
	friend class DeviceContext;//Adding DeviceContext as a friend class to Accese Private members of the class
};

