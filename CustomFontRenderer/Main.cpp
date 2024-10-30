#if defined DEBUG || defined _DEBUG
//#include <vld.h>
#endif

#include <SDL2/SDL.h>
#include "BaseApplication.h"

int SDL_main(int argv, char** args)
{
	BaseApplication app{ "currentTitle", 1080, 720 };
	app.Run();

	return 0;
}