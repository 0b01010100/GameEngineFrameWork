#pragma once
#include <d3d11.h>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	bool load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	UINT getSizeVertexList();
	bool release();
	~VertexBuffer();
private:
	UINT m_size_vertex;//Size of Vertex in byts
	UINT m_size_list;//Size of list in byts
private:
	ID3D11Buffer* m_buffer; //The buffer
	ID3D11InputLayout* m_layout;
private:
	friend class DeviceContext;
};

