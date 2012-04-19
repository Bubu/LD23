#include <ProgamManager.h>
#include <stdio.h>
#include <iostream>
DW::ProgramManager::ProgramManager():gFXEngine(0)
{
	
}

DW::ProgramManager::~ProgramManager()
{
	
}
bool DW::ProgramManager::run()
{
	//OS
	init();
	int state=1;
	//Menu
	gFXEngine->drawMenu(_menu);
	while(state)
	{
		eventManager->check();
		if (eventManager->fullScreenToggled()) 
		{
			printf("TFS \n");
			gFXEngine->toggleFullscreen();		
		}
		
		int time=100;
		switch(state)
		{
			case 1: state=menuTick();break;
			case 2: state=ingameTick(time);break;
			case 3: state=outroTick();break;
			case 4: state=ingamePausedTick();break;
		};
		//gFXEngine->drawHUD(_tacBall);
		gFXEngine->frameEnd();		
		if(eventManager->getExit()==true)state=0;
	}
   return true;
}

int DW::ProgramManager::menuTick() 
{
	//eventManager->check();
	bool down=false;
	bool up=false;
	bool button=false;
	
	for (int i=0;i<2;i++)
	{
		//0=centered; 1=up; 2=rightup; 3=right; 4=rightdown; 5=down; 6=leftfown; 7=left; 8=leftup
		int hat=eventManager->hat(i);
		if (hat==1||hat==2||hat==8)up=true;
		if (hat==5||hat==4||hat==6)down=true;
		if (eventManager->buttonPressed(i)||
		    eventManager->startPressed(i)||
		    eventManager->selectPressed(i))button=true;
		//if (hat!=0)printf("hat %i\n",hat);
	}
	//printf("MenueTick \n");
	if (down&&!up)_menu.setDown();
	if (up&&!down)_menu.setUp();
	if (button)
		switch(_menu.getState())
		{
			case 0: return 0;
			case 1: return 2;	
		}
	gFXEngine->drawMenu(_menu);
	return 1;
}

int DW::ProgramManager::ingameTick(int time) 
{
	float r[2]={0.0f,0.0f};
	float a[2]={0.0f,0.0f};
	for (int i=0;i<2;i++)
	{
		
		//int Hat;//0=centered; 1=up; 2=rightup; 3=right; 4=rightdown; 5=down; 6=leftfown; 7=left; 8=leftup
		switch(eventManager->hat(i))
		{
			case 1: a[i]=+1.0f;break;
			case 3: r[i]=-1.0f;break;
			case 5: a[i]=-1.0f;break;
			case 7: r[i]=+1.0f;break;	
		}
	}
	std::cout<<"[("<<a[0]<<" "<<r[0]<<") ("<<a[1]<<" "<<r[1]<<")]\n ";
	_tacBall.tick(/*time*/100,a,r);
	gFXEngine->drawIngame(_tacBall);
	bool pause=false;
	for (int i=0;i<2;i++)if(eventManager->startPressed(i))pause=true;
	if (pause) return 4;
	
	bool chatwin=false;
	for (int i=0;i<2;i++)if(eventManager->selectPressed(i))chatwin=true;
	if (chatwin) return 3;
	
	return 2;
}
int DW::ProgramManager::ingamePausedTick() 
{
	gFXEngine->drawIngamePaused(_tacBall);
	bool resume=false;
	for (int i=0;i<2;i++)if(eventManager->startPressed(i))resume=true;
	if (resume) return 2;
	
	bool end=false;
	for (int i=0;i<2;i++)if(eventManager->selectPressed(i))end=true;
	if (end) return 1;
	return 4;
}
int DW::ProgramManager::outroTick() 
{
	gFXEngine->drawOutro();
	
	bool button=false;
	
	for (int i=0;i<2;i++)
		if (eventManager->buttonPressed(i)||
		    eventManager->startPressed(i)||
		    eventManager->selectPressed(i))button=true;
	if (button)return 1;
	
	return 3;
}

bool DW::ProgramManager::init()
{
	settings = new Settings();
	eventManager= new EventManager();
	eventManager->init(settings);
	gFXEngine= new GFXEngine();
	gFXEngine->init(settings);
	return true;
}

