#include "MyGame.h"
#include "Player.h"

MyGame::MyGame ( )
{
}

MyGame::~MyGame ( )
{
}

void MyGame::onCreate ( )
{
	Game::onCreate ( );

	auto mesh = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/house.obj" );
	auto tex = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/wood.jpg" );

	auto material = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	material->addTexture ( tex );

	m_entity = getWorld ( )->createEntity<Entity> ( );
	auto meshComponent = m_entity->createComponent<MeshComponent> ( );
	meshComponent->setMesh ( mesh );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );
}
void MyGame::onUpdate ( f32 deltaTime )
{

	Game::onUpdate ( deltaTime );
	m_rotation += 1.57 * deltaTime;

	m_entity->getTransform ( )->setRotation(Vector3D(0, m_rotation, 0));
}
