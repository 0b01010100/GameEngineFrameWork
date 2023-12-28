#pragma once

#include <DX3D/Prerequisites.h>

class Entity
{
public:
	Entity ( );
	virtual ~Entity ( );
	void release ( );

	World* getWorld ( );
protected:
	virtual void onCreate ( );
	virtual void onUpdate ( f32 deltaTime ) {}
protected:
	size_t m_typeId = 0;
	World* m_world = nullptr;
	friend class World;
};

