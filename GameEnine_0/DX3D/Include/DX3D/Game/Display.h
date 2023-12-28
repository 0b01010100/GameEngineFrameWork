#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Window/Window.h>

class Display : public Window
{
public:
	Display ( Game* game );
	~Display ( );
protected:
	virtual void onSize ( const Rect& size );
private:
	SwapChainPtr m_swapChain;
	Game* m_game = nullptr; 
private:
	friend class GraphicsEngine;


};