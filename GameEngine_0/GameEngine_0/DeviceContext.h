#pragma once
#include <d3d11.h>
#include "SwapChain.h"
#include "VertexBuffer.h"
class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	//Creates a Shape using vertices
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	//Creates a Shape by using less Vertices 
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	void setViewportSize(UINT width, UINT height);

	bool release();
	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context = nullptr;
};


