#include "d3d11.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"


VertexBuffer::VertexBuffer() :m_layout(0), m_buffer(0)
{
}

bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader)
{
	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;//The D3D11_USAGE is used to specfy how a resource, like a texture will be used in the graphics pipline. Dettermine stratagies for memory managament and how the GPU and CPU will permoform.
	buff_desc.ByteWidth = size_vertex * size_list;//The sum of the size of each Vertex class Instaiated . 
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//This enables this to act as a Vertex buffer. Vertex Buffers provide the input data for vertex shaders and are manily used to store the posiotions, colors, normals, and other data about a vertex.
	buff_desc.CPUAccessFlags = 0; // No CPU access are need, GPU only.
	buff_desc.MiscFlags = 0;//Additionall information gose here
	//This class is used to Create and Update classes related to the directX
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;//adds the Vertices class list to a Pointer to raw data for. The location that the pSysMem pointer is pointing at is specifically for the DircetsX buffers or Vertex Classes.  

	m_size_vertex = size_vertex; // The Size of the Vertex Class 
	m_size_list = size_list; // The Size of all of the Vertex Classes.
	//Create a buffer 
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		return false;
	}
	//Creating Attributes for the Vertex
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POS", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"POS", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"COLOR", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,D3D11_INPUT_PER_VERTEX_DATA ,0}
		//{"COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,D3D11_INPUT_PER_VERTEX_DATA ,0}
		
		
	};
	//Gets the size of the Layout array
	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
	{
		return false;
	}

	return true;
}
//Return the SIze of the Vertex List
UINT VertexBuffer::getSizeVertexList()
{
	
	return this->m_size_list;
}
//Realses resources.
bool VertexBuffer::release()
{
	if(m_layout != NULL) m_layout->Release();
	m_buffer->Release();
	delete this;
	return true;
}


VertexBuffer::~VertexBuffer()
{
}
