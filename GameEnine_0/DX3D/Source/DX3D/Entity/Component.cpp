#include <DX3D/Entity/Component.h>
#include <DX3D/Entity/Entity.h>

Component::Component ( )
{
}

Component::~Component ( )
{
}

void Component::release ( )
{
	m_entity->removeComponent ( m_typeId );
}
