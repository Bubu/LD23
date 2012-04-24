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

	const int mountain_count = 21+4+21+22;
	int mountain[] = {792,793,998,996,999,804,806,985,807,795,774,768,769,786,798,803,801,800,982,809,977,974,962,972,1014,
	1007,995,349,1018,1009,1010,371,374,351,333,331,321,347,992,1000,340,638,635,325,352,353,
	586,590,578,621,615,628,631,1278,341,1276,791,589,594,601,523,571,566,513,518,551,619,616};
	for(int i = 0; i < mountain_count; i++)
	{
		_level0[mountain[i]].type = 1; 
		_level0[mountain[i]].color = Vector3f(0.447,0.38,0.286);
		_level0[mountain[i]].height = 1.1;
		_level0[mountain[i]].blocking = true;
	}

	const int grass_count = 5+11+16+87;
	int grass[] = {773,805,986,965,984,964,966,961,960,963,973,967,987,978,990,976,775,771,781,780,783,821,820,822,782,770,779,825,817,819,823,816,
	969,968,970,971,1017,1016,1019,375,372,373,335,332,334,322,320,323,327,324,326,359,356,357,623,620,358,622,610,608,611,609,555,617,546,544,557,
	547,612,521,520,522,570,602,600,603,583,579,576,577,585,584,587,
	630,625,633,348,350,338,337,342,345,344,346,634,632,339,343,1003,994,1006,1004,1005,626,624,627,637,636,639,1279,789,788,790,1001,993,785,784,787,336};
	for(int i = 0; i < grass_count; i++)
	{
		_level0[grass[i]].type = 0; 
		_level0[grass[i]].color = Vector3f(0.5,0.694,0.01);
		_level0[grass[i]].height = 1.0;
	}

	_level0[794].type = 7; 
	_level0[794].color = Vector3f(1,1,1);
	_level0[794].height = 1.0;

	_level0[772].type = 8;
	_level0[772].doorid = 0;
	_level0[772].color = Vector3f(0.30,0.20,0.15);
	_level0[772].height = 1.1;

	_level0[1002].type = 6;
	_level0[1002].item = 0;
	_level0[1002].color = Vector3f(239,183,52) / 255;
	_level0[1002].height = 1;

	_level0[1002].type = 6;
	_level0[1002].item = 1;
	_level0[1002].color = Vector3f(239,183,52) / 255;
	_level0[1002].height = 1;

	const int enemy_count = 5;
	int enemy[] = {0,976,783,784,582};
	for(int i = 0; i < enemy_count; i++)
	{
		_level0[enemy[i]].type = 10; 
		_level0[enemy[i]].color = Vector3f(1,0,0);
		_level0[enemy[i]].height = 1;
	}

	
	const int water_count = 3+51;
	int water[] = {963,960,973,829,828,831,149,1237,1236,1239,1238,745,
		744,746,731,707,711,734,717,722,720,716,732,580,613,581,614,556,558,559,483,493,449,454,487,
		457,450,448,451,462,460,461,1032,1080,1081,1035,1026,1073,1074,1072,1073,1078,1038,1083};
	for(int i = 0; i < water_count; i++)
	{
		_level0[water[i]].type = 1; 
		_level0[water[i]].color = Vector3f(0,0.2,1);
		_level0[water[i]].height = 0.98;
		_level0[water[i]].blocking = true;
	}
	
}

void World::tick(float time)
{
	
	_attack.tick(time);
	const int tri=_genie.triangle();
	if (tri>= 0 && currentLevel()[tri].type==5)
	{
		_currentLevel=(_currentLevel+1)%maxLevel;
		_player->setTriangle(currentLevel().startTile());
		_sfxEngine->playLevelSound();
	}
	
	if (tri>= 0 && currentLevel()[tri].type==10)
	{
		_currentLevel=0;
		_player->setTriangle(currentLevel().startTile());
		//_sfxEngine->playDeathSound();
	}
	if (tri>= 0 && currentLevel()[tri].type==6)
	{
		//_currentLevel=0;
		//_player->setTriangle(currentLevel().startTile());
		//_sfxEngine->playKeySound();
		int key=currentLevel()[tri].item;
		
		if (key>0)
		{
		 	for (int i=0;i<currentLevel().size();i++)
			{
				if (currentLevel()[i].doorid==key)
				switch(_currentLevel)
				{
					case 0:_level0[i].blocking=false;
					case 1:_level1[i].blocking=false;
					case 2:_level2[i].blocking=false;
				}	
			}/**/
			switch(_currentLevel)
			{
				case 0:_level0[tri].type=0;_level0[tri].color=Vector3f(0.5,0.694,0.01);false;
				case 1:_level1[tri].type=0;_level1[tri].color=Vector3f(0.5,0.694,0.01);false;
				case 2:_level2[tri].type=0;_level2[tri].color=Vector3f(0.5,0.694,0.01);false;
			}
		}
	}
	
	//_attack.print()	;
}
/*const World::Level& _level(int i)
{
		
}	
**/


