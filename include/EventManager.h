#ifndef __DW__EventManager_H_INCLUDED__
#define __DW__EventManager_H_INCLUDED__
#include <Settings/Settings.h>
union SDL_Event;

namespace DW
{

class EventManager
{
  public:	
	void check();
	void init(Settings* settings);
	inline int hat(int deviceNumber)const;
	inline bool buttonPressed(int deviceNumber)const;
	inline bool startPressed(int deviceNumber)const;
	inline bool selectPressed(int deviceNumber)const;
	inline bool getExit()const;
	inline bool fullScreenToggled()const;

  protected:
	class Device
	{
	  public:
			int Hat;//0=centered; 1=up; 2=rightup; 3=right; 4=rightdown; 5=down; 6=leftfown; 7=left; 8=leftup
			bool start , select;
			bool button;
			inline void setNull();	
			inline void resetButtons();				
	} devices[2];
	bool _exit;
	unsigned int getDelay();	
	Settings * _settings;	
  private:
	void evaluate(const SDL_Event& event);
	int timeLeft();
	int _next_time;	
	unsigned int time_;
	bool _frameLimit;
	bool _fullScreenToggled;		
};

inline int EventManager::hat(int deviceNumber)const{return devices[deviceNumber].Hat;}
inline bool EventManager::startPressed(int deviceNumber)const{return devices[deviceNumber].start;}
inline bool EventManager::selectPressed(int deviceNumber)const{return devices[deviceNumber].select;}
inline bool EventManager::buttonPressed(int deviceNumber)const{return devices[deviceNumber].button;}
inline void EventManager::Device::setNull()
{
	Hat=0;
	button=false;
	start=false;
	select=false;
}
inline void EventManager::Device::resetButtons()
{
	button=false;
	start=false;
	select=false;
}
inline bool EventManager::getExit()const{return _exit;}
inline bool EventManager::fullScreenToggled()const{return _fullScreenToggled;}	
}

#endif
