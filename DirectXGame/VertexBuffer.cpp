#include "VertexBuffer.h"
#include "GraphicsEngine.h"
class SwapChain;
class DeviceContext;

VertexBuffer::VertexBuffer() : m_layout(0), m_buffer(0)
{
}
//Creates a Vertex Buffer
bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{

	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();



	//A Varable that describes how our buffer works 
	D3D11_BUFFER_DESC buff_desc = {};
	//Wheater the
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	
	buff_desc.ByteWidth = size_vertex * size_list;
	//We are telling the buffer how to bind this to the Grahpics pipline
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	//Passing a pointer to the Vertex list in Memory of were the Vertexes are located
	init_data.pSysMem = list_vertices;
	
	m_size_vertex = size_vertex;
	m_size_list = size_list;

	//Attemps to Create a buffer and checks to see if the buffer making prosses gose well.
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc/*Passing in the info of the attributtes to determine the Functionality and or values of variables the is Buffer struct*/,
		&init_data/*Passing in the posstions in memort the the list a vertices are located*/, 
		&m_buffer/*Reference to the Created Buffer*/))
	   )
	{
		return false;
	}
	//Defining Descriptor object with will define all of the Attributes for a vertex layout
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		//SEMATIC NAME 
		{"POSITION",
		//SEMANTIC INDEX 
				0,       
		//FORMAT 
		DXGI_FORMAT_R32G32B32_FLOAT,
		//INPUT SLOT
				0,
		//ALIGNED BYTE OFFSET 
				0,
		//INPUT SLOT CLASS
		D3D11_INPUT_PER_VERTEX_DATA,
		//INSTANCE DATA STEP RATE
		0,
		}
		
	};
	//Gets the Number to Attributes
	UINT size_layout = ARRAYSIZE(layout);
	//Creating a Input layout Function
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout))) 
	{
		return false;
	}

	return true;
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

bool VertexBuffer::release()
{
	m_layout->Release();
	m_buffer->Release();
	delete this;
	return true;
}

VertexBuffer::~VertexBuffer()
{
}
