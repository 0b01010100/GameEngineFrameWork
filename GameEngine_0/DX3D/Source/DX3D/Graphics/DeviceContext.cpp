#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/Texture2D.h>
//#include "Texture.h"
//#include <exception>

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

	auto rtv = swap_chain->m_rtv.Get ( );
	auto dsv = swap_chain->m_dsv.Get ( );

	m_device_context->ClearRenderTargetView( rtv, clear_color );
	m_device_context->ClearDepthStencilView( dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0 );
	m_device_context->OMSetRenderTargets(1, &rtv, dsv );
}
void DeviceContext::clearDepthStencil(const SwapChainPtr& swap_chain)
{
	auto dsv = swap_chain->m_dsv.Get ( );
	m_device_context->ClearDepthStencilView( dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0 );
}
////void DeviceContext::clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha)
////{
////	if (render_target->m_type != Texture::Type::RenderTarget) return;
////	FLOAT clear_color[4] = { red,green,blue,alpha };
////	m_device_context->ClearRenderTargetView(render_target->m_render_target_view, clear_color);
////}
////void DeviceContext::clearDepthStencil(const TexturePtr& depth_stencil)
////{
////	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;
////	m_device_context->ClearDepthStencilView(depth_stencil->m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
////
////}
////void DeviceContext::setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
////{
////	if (render_target->m_type != Texture::Type::RenderTarget) return;
////	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;
////	m_device_context->OMSetRenderTargets(1, &render_target->m_render_target_view, depth_stencil->m_depth_stencil_view);
////}
///Prep for Input Assembler Stage
//The will tell Our grahpihc card what and where to draw the verties
void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	//Size of the Vertex
	UINT stride = vertex_buffer->m_size_vertex;
	//Distance betteen Each Vertex Vairable in Computer Memory
	UINT offset = 0;
	auto buffer = vertex_buffer->m_buffer.Get ( );

	//to specify which vertex buffers to use during rendering.
	m_device_context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout.Get());
}
///Prep for Input Assembler Stage
//to specify which vertex buffers to use during rendering.
void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{

	//Give the GPU the IndexBuffer class while indicating how much computer memory(RAM) was allocated using by the Verties 
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
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
	m_device_context->VSSetShader(vertex_shader->m_vs.Get(), nullptr, 0 );
}
//Allows us to tell rendering deivice what PixelShader we want to to use.
void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_ps.Get(), nullptr, 0 );
}


//void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, const unsigned int num_textures)
//{
//	ID3D11ShaderResourceView* list_res[32];
//	ID3D11SamplerState* list_sampler[32];
//	for (unsigned int i = 0; i < num_textures; i++)
//	{
//		list_res[i] = texture[i]->m_shader_res_view;
//		list_sampler[i] = texture[i]->m_sampler_state;
//	}
//	m_device_context->VSSetShaderResources(0, num_textures, list_res);
//	m_device_context->VSSetSamplers(0, num_textures, list_sampler);
//}
//
void DeviceContext::setTexture( const Texture2DPtr* texture, unsigned int num_textures )
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view.Get();
		list_sampler[i] = texture[i]->m_sampler_state.Get ( );
	}

	m_device_context->VSSetShaderResources(0, num_textures, list_res);//Set Shader resource for the Vertex shader so the textue can be use in the shader
	m_device_context->VSSetSamplers(0, num_textures, list_sampler);//Set the way the texture should be samapled 

	m_device_context->PSSetShaderResources(0, num_textures, list_res);
	m_device_context->PSSetSamplers(0, num_textures, list_sampler);
}
//Allows us to pass in Constant buffer has so it can be usable for the Vertex shaders in HLSL 
void DeviceContext::setConstantBuffer (const ConstantBufferPtr& buffer )
{
	auto buf = buffer->m_buffer.Get ( );
	m_device_context->VSSetConstantBuffers ( 0, 1, &buf );
	m_device_context->PSSetConstantBuffers ( 0, 1, &buf );
}