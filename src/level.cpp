#include <level.h>
#include <tile.h>
#include <math.h>
#include <iostream>

Level::Level(int n,int id):_tg(TriangleGraph::TriangleGraph(n)),_size(_tg.size()),id(id)
{
	_tiles = new Tile[_size]();
	for (int i=0;i<_size;i++)if (i%2==0)_tiles[i].color=Vector3f(1,0.5,0.3);
	
}

Level::~Level()
{
	delete[] _tiles;
}
/*void Level::draw()const
{

}*/
