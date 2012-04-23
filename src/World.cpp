#include <World.h>

World::World():_level0(Level(3,0)),_level1(Level(3,1)),_level2(Level(3,2)),maxLevel(3),_currentLevel(0)
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

	const int mountain_count = 21+4;
	int mountain[] = {792,793,998,996,999,804,806,985,807,795,774,768,769,786,798,803,801,800,982,809,977,974,962,972,1014};
	for(int i = 0; i < mountain_count; i++)
	{
		_level0[mountain[i]].type = 1; 
		_level0[mountain[i]].color = Vector3f(0.447,0.38,0.286);
		_level0[mountain[i]].height = 1.1;
		_level0[mountain[i]].blocking = true;
	}

	const int grass_count = 5+11+16;
	int grass[] = {773,805,986,965,984,964,966,961,960,963,973,967,987,978,990,976,775,771,781,780,783,821,820,822,782,770,779,825,817,819,823,816};
	for(int i = 0; i < grass_count; i++)
	{
		_level0[grass[i]].type = 0; 
		_level0[grass[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[grass[i]].height = 1.0;
	}

	_level0[794].type = 7; 
	_level0[794].color = Vector3f(1,1,1);
	_level0[794].height = 1.0;

	_level0[772].type = 7; 
	_level0[772].color = Vector3f(0.30,0.20,0.15);
	_level0[772].height = 1.1;

	
	const int water_count = 3;
	int water[] = {963,960,973};
	for(int i = 0; i < water_count; i++)
	{
		_level0[water[i]].type = 1; 
		_level0[water[i]].color = Vector3f(0,0.2,1);
		_level0[water[i]].height = 0.98;
		_level0[water[i]].blocking = true;
	}
	
}

/*const World::Level& _level(int i)
{
		
}	
**/


