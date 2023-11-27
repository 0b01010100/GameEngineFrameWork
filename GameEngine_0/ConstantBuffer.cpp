#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>
ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system) : m_system(system)
{
	//Allows us to customize the behavior of our buffer
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;//The D3D11_USAGE is used to specfy how a resource, like a texture will be used in the graphics pipline. Dettermine stratagies for memory managament and how the GPU and CPU will permoform.
	buff_desc.ByteWidth = size_buffer;//The sum of the size of each Vertex class Instaiated . 
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//Tells the 
	buff_desc.CPUAccessFlags = 0; // No CPU access are need, GPU only.
	buff_desc.MiscFlags = 0;//Additionall information gose here
	//This class is used to Create and Update classes related to the directX
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;//adds the ConstantButter class to a Pointer to raw data.The location that the pSysMem pointer is pointing at is specifically for the DircetsX buffers or Vertex Buffer Classes.  
	//Create a buffer 
	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("ConstantBuffer not Created successfully");
	}

}
//Updates the data in GPU memory
void ConstantBuffer::update(DeviceContextPtr context, void* buffer)
{
	context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_buffer)m_buffer->Release();
}