#ifndef __EventManager_H_INCLUDED__
#define __EventManager_H_INCLUDED__

union SDL_Event;

class EventManager
{
  public:	
	void check();
	EventManager():	_upTyped(false),_downTyped(false),_leftTyped(false),_rightTyped(false),
					_upPressed(false),_downPressed(false),_leftPressed(false),_rightPressed(false),
					_escTyped(false),_actionPressed(false),_jumpPressed(false),_frameLimit(true),
					_fullScreenToggled(false),_keyTyped(false),_exit(false),_number(0),_maxNumber(0){}
	inline void setMaxNumber(int i){_maxNumber=i;}
	inline int number()const {return _number;}
	inline bool upPressed()const{return _upPressed;}
	inline bool downPressed()const{return _downPressed;}
	inline bool leftPressed()const{return _leftPressed;}
	inline bool rightPressed()const{return _rightPressed;}
	inline bool upTyped()const{return _upPressed;}
	inline bool downTyped()const{return _downPressed;}
	inline bool leftTyped()const{return _leftPressed;}
	inline bool rightTyped()const{return _rightPressed;}
	inline bool keyTyped()const{return _keyTyped;}
	inline bool escTyped()const{return _escTyped;}
	inline bool actionPressed()const{return _actionPressed;}
	inline bool jumpPressed()const{return _jumpPressed;}
	inline bool getExit()const{return _exit;}
	inline bool fullScreenToggled()const{return _fullScreenToggled;}
	inline float duration()const{return _duration;}
	void setTime();	
	
  private:
	unsigned int getDelay();	
	void evaluate(const SDL_Event& event);
	int timeLeft();
	//int _next_time;	
	unsigned int time_;
	unsigned int _lastTime;
	bool _frameLimit;
	
	bool _upPressed;
	bool _downPressed;
	bool _leftPressed;
	bool _rightPressed;
	bool _upTyped;
	bool _downTyped;
	bool _leftTyped;
	bool _rightTyped;
	bool _escTyped;
	bool _keyTyped;
	bool _actionPressed;
	bool _jumpPressed;
	bool _fullScreenToggled;	
	float _duration;
	bool _exit;	
	
	int _maxNumber;
	int _number;
};


#endif
