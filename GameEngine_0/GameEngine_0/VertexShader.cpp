#include "VertexShader.h"
#include "GraphicsEngine.h"


VertexShader::VertexShader()
{
}

void VertexShader::release()
{
	if(m_vs)m_vs->Release();
	delete this;
}
///Init function allows us to pass in the compiled shader code during initalizion to create a new shader
bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
		return false;

	return true;
}



VertexShader::~VertexShader()
{
}