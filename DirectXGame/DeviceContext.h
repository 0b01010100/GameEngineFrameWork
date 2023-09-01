#pragma once
#include <d3d11.h>//Importing this allows use to Access the DircetX API

class SwapChain;

class DeviceContext
{
public:
	//On Initialization the m_device_context  is asgned to device_context
	DeviceContext(ID3D11DeviceContext* device_context);

	bool ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	//Called to release the resources used back to the prosscesors. 
	bool release();

	~DeviceContext();
private:
	//This Variable is a pointer for another variable that dose most of the heavy work like controlling va lot of stages of the rendering process, such as setting shaders, rendering states, Updating Buffer, and sending geometry data to the GPU for processing.
	ID3D11DeviceContext* m_device_context = nullptr;
};

