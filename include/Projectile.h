#ifndef __Projectile_H_INCLUDED__
#define __Projectile_H_INCLUDED__
#include <Matrix3x3f.h>
#include <Vector3f.h>
//#include <SDL/SDL.h>
//#include <iostream>
//#include <iostream>
class Projectile
{
  public:
	inline bool isAlive()const {return _age<_lifeTime;}
	inline Vector3f pos()const 
	{
		return _r*Vector3f(0.0f,cos(-_age*_revolutionsPerSecond*6.283185307179586476925286766559f),
								sin(-_age*_revolutionsPerSecond*6.283185307179586476925286766559f));
	}
	inline void tick(float time){_age+=time;}
	Projectile(float revolutionsPerSecond, float lifeTime, const Matrix3x3f& r )
		:_age(0.0f),_revolutionsPerSecond(revolutionsPerSecond),_lifeTime(lifeTime),_r(r){}
  private:	
	float _age;
	float _revolutionsPerSecond;
	float _lifeTime;
	Matrix3x3f _r;
};

#endif
