#include "MyGame.h"

int main()
{
	try
	{
		MyGame game = {};
		game.run ( );
	}
	catch (const std::exception&)
	{
		return -1 ;
	}
	return 0;
}