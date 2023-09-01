#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context = nullptr) : m_device_context(device_context)
{
}

bool DeviceContext::ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	//Color Setting List
	FLOAT clear_color[4] = {red, green, blue, alpha};
	//Using the Color Settings list, the Color of the target view will be changed before another frame. This is just clearing the window before more stuff is painted on to it.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv,clear_color);


	return true;
}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return TRUE;
}

DeviceContext::~DeviceContext()
{
}
