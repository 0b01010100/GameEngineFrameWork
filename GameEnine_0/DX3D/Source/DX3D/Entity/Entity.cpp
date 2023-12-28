#include <DX3D/Entity/Entity.h>
#include <DX3D/Game/World.h>

Entity::Entity ( )
{

}

Entity::~Entity ( )
{

}

void Entity::release ( )
{
	this->m_world->removeEntity ( this );
}

World* Entity::getWorld ( )
{
	return m_world;
}

void Entity::onCreate ( )
{
}
