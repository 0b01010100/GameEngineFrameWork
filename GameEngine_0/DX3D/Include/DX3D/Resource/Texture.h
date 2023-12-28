#pragma once
#include <DX3D/Resource/Resource.h>

class Texture : public Resource
{
public:
	Texture ( const wchar_t* full_path, ResourceManager* manager );
	
private:
	Texture2DPtr m_texture;
	friend class Material;
} ;

