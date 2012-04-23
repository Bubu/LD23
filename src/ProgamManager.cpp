#include <ProgamManager.h>
#include <stdio.h>
//#include <iostream>
#include <SDL/SDL.h>

#include <GL/glu.h>
#include <GFXEngine.h>
#include <SFXEngine.h>
#include <EventManager.h>
ProgramManager::ProgramManager(): _world(World()),_player(Player(_world.genie_(),_world))
{
	eventManager=new EventManager();
	gfxEngine=new GFXEngine();
	sfxEngine=new SFXEngine();
}

ProgramManager::~ProgramManager()
{
	
}
bool ProgramManager::run()
{
	int state=1;
	eventManager->setTime();
	gfxEngine->drawMenu(_menu);
	while(state)
	{
		eventManager->check();
		if (eventManager->fullScreenToggled()) gfxEngine->toggleFullscreen();	
		int time=100;
		switch(state)
		{
			case 1: state=menuTick();break;
			case 2: state=ingameTick();break;
			case 3: state=outroTick();break;
			case 4: state=ingamePausedTick();break;
		};
		SDL_GL_SwapBuffers();		
		if(eventManager->getExit()==true)state=0;
	}
   return true;
}

int ProgramManager::menuTick() 
{
	const bool down=eventManager->downPressed();
	const bool up=eventManager->upPressed();
	bool button=eventManager->keyTyped();
	
	if (down&&!up)_menu.setDownMain();
	if (up&&!down)_menu.setUpMain();
	if (button)
		switch(_menu.getStateMain())
		{
			case 0: return 0;
			case 1: return 2;	
		}
	gfxEngine->drawMenu(_menu);
	return 1;
}

int ProgramManager::ingameTick() 
{
	
	const bool down=eventManager->downPressed();
	const bool up=eventManager->upPressed();
	const bool right=eventManager->rightPressed();
	const bool left=eventManager->leftPressed();
	const bool jumped=eventManager->jumpPressed();
	const bool action=eventManager->actionPressed();
	const float t=eventManager->duration();
	//std::cout<<"time:"<<t<<"\n";
	float move=0.0f; if (down)move-=t*0.02f; if (up)   move+=t*0.02f;
	float roty=0.0f; if (left)roty-=t*0.03f; if (right)roty+=t*0.03f;
	float jump=0.0f; if (jumped)jump+=t;
	//if (right&&!left)_player.addRoty( t*10);
	//if (left&&!right)_player.addRoty(-t*10);
	//if (down&&!up)_player.moveForward(-t*5,_world.level(_world.current()).triangleGraph());
	//if (up&&!down)_player.moveForward( t*5,_world.level(_world.current()).triangleGraph());
	_player.tick(t,move,jump,roty,action);
	_world.tick(t);
	//std::cout<<"t: "<<t<<" roty: "<<_player.roty()<<"\n";
	sfxEngine->playTestSound(eventManager->jumpPressed());
	gfxEngine->drawIngame(_world,_player);
	if (eventManager->escTyped())
	{	
		_menu.setFirstState(); 
		return 4;
	}
	return 2;
}
int ProgramManager::ingamePausedTick() 
{
	
	const bool down=eventManager->downTyped();
	const bool up=eventManager->upTyped();
	bool button=eventManager->keyTyped();
	
	if (down&&!up)_menu.setDownPause();
	if (up&&!down)_menu.setUpPause();
	if (button)
		switch(_menu.getStatePause())
		{
			case 0: return 1;
			case 1: return 2;	
		}
	if (eventManager->escTyped())return 2;
	gfxEngine->drawIngamePaused(_world,_player,_menu);
	return 4;
}
int ProgramManager::outroTick() 
{
	//gfxEngine->drawOutro();
	
	if(eventManager->keyTyped()|| eventManager->escTyped())return 1;

	return 3;
}

