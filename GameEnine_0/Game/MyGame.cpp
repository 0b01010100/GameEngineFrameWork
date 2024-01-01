#include "MyGame.h"
#include "Player.h"
#include <time.h>


MyGame::MyGame ( )
{
}

MyGame::~MyGame ( )
{
}

void MyGame::onCreate ( )
{
	Game::onCreate ( );


	srand ( (ui32)time ( nullptr ) );


	auto mesh = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/house.obj" );
	auto terrain = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/terrain.obj" );
	auto sphere = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/sphere.obj" );
	auto box = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/Box.obj" );
	auto sponza = getResourceManager()->createResourceFromFile< Mesh > ( L"Assests/Meshes/sponza_basic.obj" );


	auto tex = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/wood.jpg" );
	auto sky = getResourceManager ( )->createResourceFromFile< Texture > ( L"Assests/Textures/sky.jpg" );

//	auto floor = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_floor_a_diff.jpg" );

	auto brick = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_bricks_a_diff.jpg" );
	auto arch = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_arch_diff.jpg" );
	auto colA = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_column_a_diff.jpg" );
	auto colB = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_column_b_diff.jpg" );
	auto colC = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_column_c_diff.jpg" );
	auto flagPole = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_flagpole_diff.jpg" );
	auto floor = getResourceManager()->createResourceFromFile< Texture > ( L"Assests/Textures/Sponza/sponza_floor_a_diff.jpg" );




	auto brickMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	brickMat->addTexture ( brick );

	auto archMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	archMat->addTexture ( arch );

	auto colAMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	colAMat->addTexture ( colA );

	auto colBMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	colBMat->addTexture ( colB );

	auto colCMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	colCMat->addTexture ( colC );

	auto flagPoleMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	flagPoleMat->addTexture ( flagPole );

	auto floorMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	floorMat->addTexture ( floor );

	auto material = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/MyMaterial.hlsl" );
	material->addTexture ( tex );
	auto skyMat = getResourceManager ( )->createResourceFromFile< Material > ( L"Assests/Shaders/SkyBox.hlsl" );
	skyMat->addTexture (	sky   );
	//skybox
	skyMat->setCullMode ( CullMode::Front );
	{
		auto entity = getWorld ( )->createEntity<Entity> ( );
		auto meshComponent = entity->createComponent<MeshComponent> ( );
		auto transform = entity->getTransform ( );
		meshComponent->setMesh ( sphere );
		meshComponent->addMaterial ( skyMat );

		transform->setScale ( Vector3D ( 1000, 1000, 1000 ) );
	}
	brickMat->setCullMode ( CullMode::None ); 
	archMat->setCullMode ( CullMode::None );
	//terrain
	{
		auto entity = getWorld ( )->createEntity<Entity> ( );
		auto meshComponent = entity->createComponent<MeshComponent> ( );
		meshComponent->setMesh ( terrain );
		meshComponent->addMaterial ( floorMat );
	}
	//sponza
	{
		auto entity = getWorld ( )->createEntity<Entity> ( );
		auto meshComponent = entity->createComponent<MeshComponent> ( );
		meshComponent->setMesh ( sponza );
		
		meshComponent->addMaterial ( archMat );
		meshComponent->addMaterial ( brickMat );
		meshComponent->addMaterial ( colAMat );
		meshComponent->addMaterial ( colBMat );
		meshComponent->addMaterial ( colCMat );
		meshComponent->addMaterial ( flagPoleMat );
	}

	//boxes
	for (int x = -7; x <= 7; x++)
	{
		for (int y = -7; y <= 7; y++)
		{
			auto entity = getWorld ( )->createEntity<Entity> ( );
			auto meshComponent = entity->createComponent<MeshComponent> ( );
			auto transform = entity->getTransform ( );
			meshComponent->setMesh ( box );
			meshComponent->addMaterial ( material );

			transform->setPosition ( Vector3D ( (rand ( ) % 40 ) - 20.0f , 0, ( (rand ( ) % 40) - 20.0f )));
			transform->setRotation (Vector3D (0,( (rand ( ) % 40) - 20.0f ), 0));
			transform->setScale ( Vector3D ( 1, 1, 1 ) );


		}
	}

	m_entity = getWorld ( )->createEntity<Entity> ( );
	auto meshComponent = m_entity->createComponent<MeshComponent> ( );
	meshComponent->setMesh ( mesh );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );
	meshComponent->addMaterial ( material );


	getWorld ( )->createEntity<Player> ( );;


	getInputSystem ( )->lockCursor ( m_looked );
}
void MyGame::onUpdate ( f32 deltaTime )
{

	Game::onUpdate ( deltaTime );
	m_rotation += 1.57 * deltaTime;

	//m_entity->getTransform ( )->setRotation(Vector3D(0, m_rotation, 0));

	if (getInputSystem ( )->isKeyDown ( Key::Escape )) 
	{
		m_looked = !m_looked;
		getInputSystem ( )->lockCursor ( m_looked  );
	}
}
