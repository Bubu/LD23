#ifndef __DW__ProgramManager_H_INCLUDED__
#define __DW__ProgramManager_H_INCLUDED__

#include <GFX/GFXEngine.h>
#include <Event/EventManager.h>
#include <Menu.h>
#include <TacBall/TacBall.h>
#include <Settings/Settings.h>

namespace DW
{
class ProgramManager
{
  public:
	ProgramManager();
	~ProgramManager();
	bool run();
	
  private:	
	bool init();
	
	
	int menuTick();
	int ingameTick(int time);
	int ingamePausedTick();
	int outroTick();
	
	Settings * settings;
	GFXEngine* gFXEngine;
	EventManager* eventManager;
	Menu _menu;
	TacBall _tacBall;
};	
}
#endif
