#include <World.h>
#include <iostream>
World::World():_level0(Level(3,0)),_level1(Level(3,1)),_level2(Level(3,2)),maxLevel(3),_currentLevel(0),_attack(Projectile(0.0f, 0.0f, Matrix3x3f()))
{
	_level0[997].color= Vector3f(1,0,0); //start tile

	const int end_count = 6;
	int end[] = {693,1119,1103,1141,671,655};
	for(int i = 0; i < end_count; i++)
	{
		_level0[end[i]].type = 5; 
		_level0[end[i]].color = Vector3f(1,0,1);
		_level0[end[i]].height = 1.01;

	}

	const int mountain_count = 10;
	int mountain[] = {792,793,998,996,999,804,806,985,807,795};
	for(int i = 0; i < mountain_count; i++)
	{
		_level0[mountain[i]].type = 1; 
		_level0[mountain[i]].color = Vector3f(0.447,0.38,0.286);
		_level0[mountain[i]].height = 1.1;
		_level0[mountain[i]].blocking = true;
	}

	const int grass_count = 5;
	int grass[] = {773,805,986,965,984};
	for(int i = 0; i < grass_count; i++)
	{
		_level0[grass[i]].type = 0; 
		_level0[grass[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[grass[i]].height = 1.0;
	}

	_level0[794].type = 7; 
	_level0[794].color = Vector3f(1,1,1);
	_level0[794].height = 1.0;
	
}

void World::tick(float time)
{
	
	_attack.tick(time);
	const int tri=_genie.triangle();
	if (tri>= 0 && currentLevel()[tri].type==5)
	{
		_currentLevel=(_currentLevel+1)%maxLevel;
		_player->setTriangle(currentLevel().startTile());
	}
	//_attack.print()	;
}
/*const World::Level& _level(int i)
{
		
}	
**/


