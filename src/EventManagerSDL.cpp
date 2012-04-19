#include <Event/EventManager.h>
#include <GFX/Shader.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
//#ifdef WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
//#endif



unsigned int DW::EventManager::getDelay()
{
	const unsigned int fps=30;
	unsigned int  _time=SDL_GetTicks();
	unsigned int  _optTime=time_+1000/fps;
	int _Delay=_optTime-_time;
	if (_Delay<0)
	{
		_Delay=0;
		_optTime=_time;
	}
	time_=_optTime;
	
	return _Delay;
}

/*int DW::EventManager::getDeviceNumber()
{
	return SDL_NumJoysticks();
}*/	
void DW::EventManager::init(Settings * settings)
{
	_settings = settings;
	int _width = _settings->screenWidth();
	int _height = _settings->screenHeigth();

	//std::cout<<"Huhuh"<<std::endl;
	_frameLimit=true;
	SDL_Surface *screen;
	//std::cout<<"INit: "<<SDL_Init (SDL_INIT_VIDEO| SDL_INIT_JOYSTICK)<<"\n--\n"<<SDL_GetError()<<"++\n";
	SDL_Init(SDL_INIT_VIDEO);
	//if (SDL_InitSubSystem (SDL_INIT_VIDEO) < 0) return false;
	atexit (SDL_Quit);
	
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int bpp = info->vfmt->BitsPerPixel;
	
	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);*/
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	int flags = SDL_OPENGL;
	if (_settings->fullScreen())
		flags = flags ^ SDL_FULLSCREEN;
	
	if ((screen = SDL_SetVideoMode(_width, _height, bpp, flags)) == 0)return;
	SDL_WM_SetCaption("TacBall", NULL);
	//SDL_Init(0);<= ist in schon in der Grafik ini drin || unsauber
	SDL_InitSubSystem(SDL_INIT_TIMER);
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	//atexit(SDL_Quit);	^^
	SDL_Joystick *joystick;
	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);
	
	GLenum err = glewInit();
	//std::cout<<"glewInit()"<<err<<std::endl;
	if (GLEW_OK != err) {
		std::cout << "GLEW error: " << glewGetErrorString(err);
		exit(-1);
	}

	if (!GLEW_VERSION_2_0) {
		std::cout << "OpenGL2 is not supported";
		exit(-1);
	}
	if(TTF_Init()==-1) 
	{
		std::cout << "TTF_Init"<< TTF_GetError()<< std::endl;
		exit(2);
	}
	
	_next_time = 0;
	time_=SDL_GetTicks();
}

int  DW::EventManager::timeLeft()
{
	//static Uint32 next_time = 0;
	int now = SDL_GetTicks();
	if ( _next_time <= now ) 
	{
		_next_time = now+30;
		return(0);
	}
	return(_next_time-now);
}
void DW::EventManager::check()
{
	for (int i=0;i<2;i++)devices[i].resetButtons();
	_exit=false; _fullScreenToggled= false;
	SDL_Event event;
	if (_frameLimit)SDL_Delay(getDelay());
	else time_=SDL_GetTicks();
	//SDL_Delay(timeLeft());
	//SDL_WaitEvent(&event);
	//evaluate(event);
	while(SDL_PollEvent(&event)) evaluate(event);        
}

void DW::EventManager::evaluate(const SDL_Event& event)
{  
	switch(event.type)
	{  
		case SDL_KEYUP: 
			/*case SDL_HAT_CENTERED:  devices[event.jaxis.which%2].Hat=0; break;
				case SDL_HAT_UP:        devices[event.jaxis.which%2].Hat=1; break;
				case SDL_HAT_RIGHTUP:   devices[event.jaxis.which%2].Hat=2; break;
				case SDL_HAT_RIGHT:     devices[event.jaxis.which%2].Hat=3; break;
				case SDL_HAT_RIGHTDOWN: devices[event.jaxis.which%2].Hat=4; break;
				case SDL_HAT_DOWN:      devices[event.jaxis.which%2].Hat=5; break;
				case SDL_HAT_LEFTDOWN:  devices[event.jaxis.which%2].Hat=6; break;
				case SDL_HAT_LEFT:      devices[event.jaxis.which%2].Hat=7; break;
				case SDL_HAT_LEFTUP:    devices[event.jaxis.which%2].Hat=8; break;*/
			switch (event.key.keysym.sym) 
			{
				case SDLK_UP:     
				case SDLK_DOWN:  
				case SDLK_RIGHT:  
				case SDLK_LEFT:   devices[0].Hat=0;       break;
				//case SDLK_RETURN: devices[0].button=true; break;
				case SDLK_w:     
				case SDLK_s:      
				case SDLK_d:      
				case SDLK_a:      devices[1].Hat=0;       break;
				//case SDLK_LCTRL:  devices[1].button=true; break;
			};
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) 
			{
				case SDLK_UP:     devices[0].Hat=1;       break;
				case SDLK_DOWN:   devices[0].Hat=5;       break;
				case SDLK_RIGHT:  devices[0].Hat=3;       break;
				case SDLK_LEFT:   devices[0].Hat=7;       break;
				case SDLK_RETURN: devices[0].button=true; break;
				case SDLK_w:      devices[1].Hat=1;       break;
				case SDLK_s:      devices[1].Hat=5;       break;
				case SDLK_d:      devices[1].Hat=3;       break;
				case SDLK_a:      devices[1].Hat=7;       break;
				case SDLK_LCTRL:  devices[1].button=true; break;
				case SDLK_F1:	_fullScreenToggled = true;	break;	
				case SDLK_ESCAPE: _exit=true; break;
				
			};
			break;	
		case SDL_QUIT:
			_exit=true;
			break;
		/*case SDL_JOYAXISMOTION:
			if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) 
			{
				switch(event.jaxis.axis)
				{
					case 0:devices[event.jaxis.which%2].LXAxis=event.jaxis.value;break;
					case 1:devices[event.jaxis.which%2].LYAxis=event.jaxis.value;break;
					case 2:devices[event.jaxis.which%2].RXAxis=event.jaxis.value;break;
					case 3:devices[event.jaxis.which%2].RYAxis=event.jaxis.value;break;	
				}
			}	
			break;*/
		case SDL_JOYBUTTONDOWN:  
			switch(event.jbutton.button) 
			{
				/*case 0: devices[event.jaxis.which%2].button=true;      break;
				case 1: devices[event.jaxis.which%2].button=true;      break;
				case 2: devices[event.jaxis.which%2].button=true;      break;
				case 3: devices[event.jaxis.which%2].button=true;      break;
				case 4: devices[event.jaxis.which%2].button=true;     break;
				case 5: devices[event.jaxis.which%2].button=true;     break;
				case 6: devices[event.jaxis.which%2].button=true;     break;
				case 7: devices[event.jaxis.which%2].button=true;     break;*/
				case 8: devices[event.jaxis.which%2].select=true; break;
				case 9: devices[event.jaxis.which%2].start=true;  break;
					
			}
			break;
		case SDL_JOYHATMOTION: 
			switch(event.jhat.value)
			{
				case SDL_HAT_CENTERED:  devices[event.jaxis.which%2].Hat=0; break;
				case SDL_HAT_UP:        devices[event.jaxis.which%2].Hat=1; break;
				case SDL_HAT_RIGHTUP:   devices[event.jaxis.which%2].Hat=2; break;
				case SDL_HAT_RIGHT:     devices[event.jaxis.which%2].Hat=3; break;
				case SDL_HAT_RIGHTDOWN: devices[event.jaxis.which%2].Hat=4; break;
				case SDL_HAT_DOWN:      devices[event.jaxis.which%2].Hat=5; break;
				case SDL_HAT_LEFTDOWN:  devices[event.jaxis.which%2].Hat=6; break;
				case SDL_HAT_LEFT:      devices[event.jaxis.which%2].Hat=7; break;
				case SDL_HAT_LEFTUP:    devices[event.jaxis.which%2].Hat=8; break;
			}
			break;
			
			
	}
}

