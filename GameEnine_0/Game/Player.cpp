#include "Player.h"

Player::Player ( )
{
}

Player::~Player ( )
{
}

void Player::onCreate ( )
{
	Entity::onCreate ( );
	m_entity = getWorld ( )->createEntity<Entity> ( );
	m_entity->createComponent < Component > ();
}

void Player::onUpdate ( f32 deltaTime )
{
	Entity::onUpdate ( deltaTime );
	m_elapsedSeconds += deltaTime;

	//if (m_entity && m_elapsedSeconds >= 3.0f)
	//{
	//	m_entity->getComponent<Component> ( )->release();
	//	m_entity->release ( );
	//	m_entity = nullptr;
	//}

	m_entity->getTransform ( )->setRotation ( Vector3D ( m_elapsedSeconds, 0,0  ) );
}

