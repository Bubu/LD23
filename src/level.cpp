#include <level.h>
#include <tile.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>

Level::Level(int n,int id):_tg(TriangleGraph::TriangleGraph(n)),_size(_tg.size()),id(id)
{
	srand(17);
	_tiles = new Tile[_size]();
	/*for (int i=0;i<_size;i++)if (i%2==0) 
	{
		_tiles[i].color=Vector3f(1,0.5,0.3);
		_tiles[i].type=1;
	}*/

	for (int i=0;i<_size;i++)
	{
		if (i%2==0) _tiles[i].color=Vector3f(0.2,0.2,0.2);
	//	//_tiles[i].type=1;
	//	_tiles[i].height=(float)rand() / (RAND_MAX + 1) * (1.15 - 0.85)  + 0.85;
	}
	
}

Level::~Level()
{
	delete[] _tiles;
}
/*void Level::draw()const
{

}*/
