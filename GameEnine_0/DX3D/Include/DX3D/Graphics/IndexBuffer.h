#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <DX3D/Prerequisites.h>

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	UINT getSizeIndexList();
private:
	UINT m_size_list;
private:
	Microsoft::WRL::ComPtr < ID3D11Buffer > m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};