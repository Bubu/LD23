#include <World.h>
#include <iostream>
World::World():_level0(Level(3,0)),_level1(Level(3,1)),_level2(Level(3,2)),maxLevel(3),_currentLevel(0),_attack(Projectile(0.0f, 0.0f, Matrix3x3f()))
{
	
	
}

void World::tick(float time)
{
	
	_attack.tick(time);
	_attack.print()	;
}
/*const World::Level& _level(int i)
{
		
}	
**/


