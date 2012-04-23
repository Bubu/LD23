#ifndef __ProgramManager_H_INCLUDED__
#define __ProgramManager_H_INCLUDED__



#include <Menu.h>
#include <World.h>
#include <Player.h>
class GFXEngine;
class SFXEngine;
class EventManager;
class ProgramManager
{
  public:
	ProgramManager();
	~ProgramManager();
	bool run();
	
  private:
	int menuTick();
	int ingameTick();
	int ingamePausedTick();
	int outroTick();
	
	GFXEngine* gfxEngine;
	SFXEngine* sfxEngine;
	EventManager* eventManager;
	Menu _menu;
	World _world;
	Player _player;
};	

#endif
