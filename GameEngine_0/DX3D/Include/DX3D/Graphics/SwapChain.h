#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <DX3D/Prerequisites.h>
class SwapChain
{
public:
	//Initialize SwapChain for a window
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	
	void setFullScreen(bool fullscreen, UINT width, UINT height);
	void resize(UINT width, UINT height);
	bool present(bool vsync);

private:
	void reloadBuffers(UINT width, UINT height);
private:
	Microsoft::WRL::ComPtr < IDXGISwapChain > m_swap_chain = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView> m_rtv = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilView> m_dsv = nullptr;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

