#ifndef __World_H_INCLUDED__
#define __World_H_INCLUDED__

#include <Level.h>
#include <Genie.h>
class World
{
  public:
	World();
	
	inline int current()const{return _currentLevel;};
	const int maxLevel;	
	inline const Level& level(int i)const;
	inline const Genie& genie()const {return _genie;} 
	inline Genie& genie_(){return _genie;}
  private:
	int _currentLevel;
	Level _level0,_level1,_level2;
	Genie _genie;
	
};

const Level& World::level(int i)const
{
	switch (i)
	{
		//case 0: return _level0;
		case 1: return _level1;
		case 2: return _level2;	
	}
	return _level0;	
}
#endif
