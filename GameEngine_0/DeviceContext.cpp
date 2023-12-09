#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "RenderSystem.h"
#include <exception>
//Input Assembler Stage: 
// This stage is responsible for assembling vertex data for rendering.
// It takes vertex data from vertex buffers and prepares it for the next stages of the pipeline.
//----------------------
//
// 
// 
//Constructor Injection design pattern to obtian reference to the ID3D11DeviceContext variable.
//When DeviceContext is created the ID3D11DeviceContext should be passed in the parameters
DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) 
	: m_system(system), m_device_context(device_context)
{
}
//This will clear the Screen
void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[4] = {red,green,blue,alpha};
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}
///Prep for Input Assembler Stage
//The will tell Our grahpihc card what and where to draw the verties
void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	//Size of the Vertex
	UINT stride = vertex_buffer->m_size_vertex;
	//Distance betteen Each Vertex Vairable in Computer Memory
	UINT offset = 0;
	//to specify which vertex buffers to use during rendering.
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}
///Prep for Input Assembler Stage
//to specify which vertex buffers to use during rendering.
void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{

	//Give the GPU the IndexBuffer class while indicating how much computer memory(RAM) was allocated using by the Verties 
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	//Specifies how the Verties should be conneted to for the primitive
	//It basically helps use tell the computer how we want our primitve draw.
	/// It specifies that vertices should be connected in a triangle strip, where each triangle shares an edge with the previous one.
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Now that you have the informantion on how to draw the triangle draw it knowing the amount of Verties and what verties to draw first
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	//Specifies how the Verties should be conneted to for the primitive
	//It basically helps use tell the computer how we want our primitve draw.
	// It specifies that triangles are formed by grouping vertices in lists of three. Each group of three vertices represents a single triangle.
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//draw the primitive knowing the amount of idices the idex list has of Verties, where to start, and the size of each vertex.
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	//Specifies how the Verties should be conneted to for the primitive
	//It basically helps use tell the computer how we want our primitve draw.
	/// It specifies that vertices should be connected in a triangle strip, where each triangle shares an edge with the previous one.
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Now that you have the informantion on how to draw the triangle draw it knowing the amount of Verties and what verties to draw first
	m_device_context->Draw(vertex_count, start_vertex_index);
}
//Changing settings of the viewport.
//The view port allows us to see in this 3d world we are creating
void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	//Tell the Device we to use this view viewport
	m_device_context->RSSetViewports(1, &vp);
}
//Allows us to tell rendering deivice what VertexShader we want to to use.
void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}
//Allows us to tell rendering deivice what PixelShader we want to to use.
void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}
//Allows us to pass in Constant buffer has so it can be usable for the Vertex shaders in HLSL 
void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}
//Allows us to pass in Constant buffer has so it can be usable for the Pixel shaders in HLSL 
void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, const unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32]{};
	ID3D11SamplerState* list_sampler[32]{};
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_sampler[i] = texture[i]->m_sampler_state;
	}
	m_device_context->VSSetShaderResources(0, num_textures, list_res);
	m_device_context->VSSetSamplers(0, num_textures, list_sampler);
}

void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, const unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32]{};
	ID3D11SamplerState* list_sampler[32]{};
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_sampler[i] = texture[i]->m_sampler_state;
	}
	m_device_context->PSSetShaderResources(0, num_textures, list_res);
	m_device_context->PSSetSamplers(0, num_textures, list_sampler);
}

//Release resources
DeviceContext::~DeviceContext() 
{	
	m_device_context->Release();
}
