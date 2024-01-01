#pragma once
#include <DX3D/All.h>

class MyGame : public Game
{
public:
	MyGame ( );
	~MyGame ( );
protected:
	virtual void onCreate ( ) override;
	virtual void onUpdate ( f32 deltaTime ) override;


private:
	Entity* m_entity = nullptr;

	f32 m_rotation = 0.0f;

	bool m_looked = true;
};

