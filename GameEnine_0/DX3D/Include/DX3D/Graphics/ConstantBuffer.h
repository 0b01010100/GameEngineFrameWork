#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <DX3D/Prerequisites.h>

class ConstantBuffer//Allows us to give tranform data to the shader so the shader can position the vertex on the screen
{
public:
	ConstantBuffer(void* buffer, ui32 size_buffer, RenderSystem* m_system);
	void update( const DeviceContextPtr& context, void* buffer);
private:
	Microsoft::WRL::ComPtr < ID3D11Buffer >  m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};