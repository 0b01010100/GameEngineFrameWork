#pragma once
#include "Resource.h"
#include <d3d11.h>
#include "IndexBuffer.h"
#include <vector>

struct MaterialSlot 
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	~Mesh();
	//Allows us to get the Vertex Buffer
	const VertexBufferPtr& getVertexBuffer();
	//Allows us to get the IndexBuffer
	const IndexBufferPtr& getIndexBuffer();
	//Allows us to get the material Slot at a specifc index
	const MaterialSlot& getMaterialSlot(UINT slot);
	size_t getNumMaterialSlots();
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;

	std::vector<struct MaterialSlot> m_material_slots;
private:
	friend class DeviceContext;
};

