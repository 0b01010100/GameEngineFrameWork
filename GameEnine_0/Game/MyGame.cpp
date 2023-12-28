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
	m_entity =  getWorld ( )->createEntity<Player>( );
}

void MyGame::onUpdate ( f32 deltaTime )
{
	Game::onUpdate ( deltaTime );
}
