#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <DX3D/Prerequisites.h>

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* m_system);
	void update(DeviceContextPtr context, void* buffer);
private:
	Microsoft::WRL::ComPtr < ID3D11Buffer >  m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};