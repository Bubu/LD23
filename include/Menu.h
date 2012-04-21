#ifndef __Menu_H_INCLUDED__
#define __Menu_H_INCLUDED__

class Menu
{
  public:
	Menu():_stateMain(1),_statePause(1){}
	
	inline int getStateMain() const{return _stateMain;}
	inline void setDownMain(){_stateMain--;if (_stateMain<0)_stateMain=0;};
	inline void setUpMain(){_stateMain++;if (_stateMain>1)_stateMain=1;};
	
	inline int getStatePause() const{return _statePause;}
	inline void setDownPause(){_statePause--;if (_statePause<0)_statePause=0;};
	inline void setUpPause(){_statePause++;if (_statePause>1)_statePause=1;};
	inline void setFirstState(){_statePause=1;}
  private:
	int _stateMain;	
	int _statePause;
};

#endif
