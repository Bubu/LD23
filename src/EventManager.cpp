#include <EventManager.h>
#include <Shader.h>
#include <SDL/SDL.h>
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#include <iostream>
unsigned int EventManager::getDelay()
{
	const unsigned int fps=60;
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

void EventManager::check()
{
	//_frameLimit=false;
	SDL_Event event;
	const unsigned int now=SDL_GetTicks();
	const unsigned int timePast=now-_lastTime;
	std::cout<<"PimePast: "<<timePast<<"["<<now<<","<<_lastTime<<"\n";
	_duration=((float)(timePast))*0.01f;
	_lastTime=SDL_GetTicks();
	if (_frameLimit)SDL_Delay(getDelay());
	else time_=SDL_GetTicks();
	
	_fullScreenToggled=false;
	_escTyped=false;
	_upTyped=false;
	_downTyped=false;
	_leftTyped=false;
	_rightTyped=false;
	_escTyped=false;
	_keyTyped=false;
	_exit=false;
	
	
	while(SDL_PollEvent(&event)) evaluate(event); 
	      
}

void EventManager::evaluate(const SDL_Event& event)
{  
	switch(event.type)
	{  
		case SDL_KEYUP: 
			switch (event.key.keysym.sym) 
			{
				case SDLK_UP: 	case SDLK_w: _upPressed=false; 		break;	    
				case SDLK_DOWN:	case SDLK_s: _downPressed=false; 	break;	  
				case SDLK_RIGHT:case SDLK_d: _rightPressed=false; 	break;		 
				case SDLK_LEFT:	case SDLK_a: _leftPressed=false; 	break;
				case SDLK_SPACE:	_jumpPressed=false;				break;
				case SDLK_f: case SDLK_RCTRL: case SDLK_LCTRL:	 
				case SDLK_KP_ENTER:	case SDLK_RETURN:	
									_actionPressed=false;			break;	
			};
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) 
			{
				case SDLK_0: _number=0; break;
				case SDLK_1: if(_maxNumber>=1)_number=1; break;
				case SDLK_2: if(_maxNumber>=2)_number=2; break;
				case SDLK_3: if(_maxNumber>=3)_number=3; break;
				case SDLK_4: if(_maxNumber>=4)_number=4; break;
				case SDLK_5: if(_maxNumber>=5)_number=5; break;
				case SDLK_6: if(_maxNumber>=6)_number=6; break;
				
				case SDLK_UP: 	case SDLK_w: _upPressed=true; 	_upTyped=true;		break;	    
				case SDLK_DOWN:	case SDLK_s: _downPressed=true; _downTyped=true;	break;	  
				case SDLK_RIGHT:case SDLK_d: _rightPressed=true;_rightTyped=true; 	break;		 
				case SDLK_LEFT:	case SDLK_a: _leftPressed=true; _leftTyped=true;	break;
				case SDLK_ESCAPE: case SDLK_PAUSE:_escTyped=true; break;
				case SDLK_SPACE:	_jumpPressed=true;	_keyTyped=true;	break;
				case SDLK_f: case SDLK_RCTRL: case SDLK_LCTRL: 
				case SDLK_KP_ENTER:	case SDLK_RETURN:	
									_actionPressed=true;_keyTyped=true;	break;
				case SDLK_F1: _fullScreenToggled=true;					break;	
				
			};
			break;	
		case SDL_QUIT:_exit=true;break;
		
		case SDL_JOYBUTTONDOWN:  
			switch(event.jbutton.button) 
			{
				case 0: case 2: _number=(_number+1)%(_maxNumber+1);break;
				case 1: case 3: _number=(_number-1+_maxNumber)%(_maxNumber+1);break;
				case 4: case 5: _actionPressed=true; _keyTyped=true;break;
				case 6: case 7: _jumpPressed=true; _keyTyped=true;break;
				case 8: case 9: _escTyped=true; break;
				
			}
			break;
		case SDL_JOYBUTTONUP:  
			switch(event.jbutton.button) 
			{
				case 4: case 5: _actionPressed=false; break;
				case 6: case 7: _jumpPressed=false; break;
				//case 8: case 9: _escPressed=true; break;	
			}
			break;
		case SDL_JOYHATMOTION: 
			switch(event.jhat.value)
			{
				case SDL_HAT_CENTERED:  _upPressed=false; _downPressed=false; _leftPressed=false; _rightPressed=false; break;
				case SDL_HAT_UP:        _upPressed=true ; _downPressed=false; _leftPressed=false; _rightPressed=false; _upTyped=true; 	break;
				case SDL_HAT_RIGHTUP:   _upPressed=true ; _downPressed=false; _leftPressed=false; _rightPressed=true ; break;
				case SDL_HAT_RIGHT:     _upPressed=false; _downPressed=false; _leftPressed=false; _rightPressed=true ; _rightTyped=true;break;
				case SDL_HAT_RIGHTDOWN: _upPressed=false; _downPressed=true ; _leftPressed=false; _rightPressed=true ; break;
				case SDL_HAT_DOWN:      _upPressed=false; _downPressed=true ; _leftPressed=false; _rightPressed=false; _downTyped=true;	break;
				case SDL_HAT_LEFTDOWN:  _upPressed=false; _downPressed=true ; _leftPressed=true ; _rightPressed=false; break;
				case SDL_HAT_LEFT:      _upPressed=false; _downPressed=false; _leftPressed=true ; _rightPressed=false; _leftTyped=true;	break;
				case SDL_HAT_LEFTUP:    _upPressed=true ; _downPressed=false; _leftPressed=true ; _rightPressed=false; break;
			}
			break;
			
			
	}
}

void EventManager::setTime()
{
	time_=SDL_GetTicks();	
}
