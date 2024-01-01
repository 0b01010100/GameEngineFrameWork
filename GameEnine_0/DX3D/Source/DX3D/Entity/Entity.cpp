#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/Component.h>
#include <DX3D/Entity/TransformComponent.h>

#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>

Entity::Entity ( )
{
	m_transform = createComponent<TransformComponent> ( ); 
}

Entity::~Entity ( )
{

}

void Entity::release ( )
{
	this->m_world->removeEntity ( this );
}

void Entity::createComponentInternal ( Component* component, size_t id )
{
	auto comPtr = std::unique_ptr< Component > ( component );
	m_components.emplace ( id, std::move ( comPtr ) );
	component->m_typeId = id;
	component->m_entity = this;

	component->onCreateInternal ( );
}

Component* Entity::getComponentInternal ( size_t id )
{
	auto it = m_components.find ( id );
	if (it == m_components.end ( )) return nullptr;
	return it->second.get ( );
}

void Entity::removeComponent ( size_t id )
{
	m_components.erase ( id );
}

World* Entity::getWorld ( )
{
	return m_world;
}

InputSystem* Entity::getInputSystem ( )
{
	return m_world->getGame()->getInputSystem();
}

TransformComponent* Entity::getTransform ( )
{
	return m_transform;
}

void Entity::onCreate ( )
{
}
