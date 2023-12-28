
#include  <DX3D/Resource/ResourceManager.h>
#include  <DX3D/Resource/Mesh.h>
#include  <DX3D/Resource/Texture.h>
#include  <DX3D/Resource/Material.h>
#include <filesystem>

ResourceManager::ResourceManager( Game* game ) : m_game(game )
{
}


ResourceManager::~ResourceManager()
{
}

Game* ResourceManager::getGame ( )
{
	return m_game;
}

ResourcePtr ResourceManager::createResourceFromFileConcrete (const wchar_t* file_path)
{
	std::filesystem::path resourePath = file_path;//Assign the wchar_t path to a file system variable to make handling the path easier 
	auto ext = resourePath.extension ( );//Get the extension of the file


	auto it = m_map_resources.find( file_path );//Check if the Resource exsitst 
	//Since the find function dosn't throught errors when something is not found checking to see if it != m_map_resources.end() will check if the data we are looking for is past the end of the list meaning it not there.
	if (it != m_map_resources.end ( )) 
	{
		auto mat = std::dynamic_pointer_cast<Material>(it->second);
		if (mat) return std::make_shared<Material> ( mat, this );
		return it->second;//Return second if it was already created. 
	}
	if (not std::filesystem::exists ( resourePath )) return ResourcePtr ( );//Checks if the file Path is real

	ResourcePtr resPtr;

	if (not ext.compare ( L".obj" )) //Checks to see if the extenstion is .obj
		resPtr = std::make_shared<Mesh> ( resourePath .c_str(), this);
	else if (not ext.compare ( L".jpg" ) || not ext.compare(L".png") || not ext.compare ( L".bmp" )) //Checks to see if the extenstion is .jpg or .png or .bmp
		resPtr = std::make_shared<Texture> ( resourePath.c_str ( ), this );
	else if (not ext.compare ( L".hlsl" ) || not ext.compare ( L".fx" )) //Checks to see if the extenstion is .jpg or .png or .bmp
		resPtr = std::make_shared<Material>( resourePath.c_str ( ), this );

	if (resPtr) //Checks to see if the extenstion is .obj
	{
		m_map_resources.emplace ( file_path, resPtr );
		return resPtr;
	}
	return nullptr;
}