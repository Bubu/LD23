#ifndef __Projectile_H_INCLUDED__
#define __Projectile_H_INCLUDED__
#include <Matrix3x3f.h>
#include <Vector3f.h>
//#include <iostream>
class Projectile
{
  public:
	inline bool isAlive()const {return _age<_lifeTime;}
	inline Vector3f pos()const {return _r*Vector3f(0.0f,cos(_age*_revolutionsPerSecond),sin(_age*_revolutionsPerSecond));}
	inline void tick(float time){_age+=time;}
	Projectile(float revolutionsPerSecond, float lifeTime, const Matrix3x3f& r ):
		_age(0.0f),_revolutionsPerSecond(revolutionsPerSecond),_lifeTime(lifeTime),_r(r){}
	//inline Projectile(const Projectile& other):_revolutionsPerSecond(other._revolutionsPerSecond),_lifeTime(other._lifeTime),_r(other._r){}
	/*inline Projectile operator=(const Projectile& other)
	{
		//:_revolutionsPerSecond(other._revolutionsPerSecond),_lifeTime(other._lifeTime),_r(other._r){}
		return Projectile(other._revolutionsPerSecond,other._lifeTime,other._r);
	}*/
	/*inline void print()
	{
		std::cout<<"pr("<< _age<<","<<_revolutionsPerSecond<<","<<_lifeTime<<")\n";
		}*/
  private:	
	float _age;
	/*const */float _revolutionsPerSecond;
	/*const */float _lifeTime;
	/*const */Matrix3x3f _r;
};

#endif
