#include <DX3D/Graphics/VertexShader.h>
#include "DX3D/Graphics/RenderSystem.h"
#include <d3dcompiler.h>
#include <exception>

VertexShader::VertexShader( const wchar_t* full_path, const char* entryPoint, RenderSystem* system) : m_system(system)
{	
	Microsoft::WRL::ComPtr<ID3DBlob> blob;//Stores the compiled shader code
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;	//Hold Messages related to Erros in the Compilelations

	D3DCompileFromFile ( full_path, 0, 0, (LPCSTR)entryPoint, "vs_5_0", 0, 0, &blob, &errorBlob );//Compiles the shader then

	if (errorBlob) //checks if the Process Faild,
		DX3DWarning( "Vertex Shader " << full_path << " compiled with errors: \n" << (char*)errorBlob->GetBufferPointer() );//if so Show error on screen.
	if (not blob) //checks if the Compilation Process Faild,
		DX3DError ("Vertex Shader " << full_path << " not created successfully." );//if so Show error on screen.
	//Trys to Create a Vertex shader using the shader byte code and shader lengh. If the Process faild an error will be shon on the screen. 
	if (FAILED(m_system->m_d3d_device.Get()->CreateVertexShader ( blob->GetBufferPointer( ), blob->GetBufferSize( ), nullptr, &m_vs)))
		DX3DError ("Vertex Shader " << full_path  << " not created successfully." );
}



