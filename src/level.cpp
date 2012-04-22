#include <level.h>
#include <tile.h>
#include <math.h>
#include <iostream>

Level::Level(int n):tg(TriangleGraph::TriangleGraph(n))
{
	size = n;
	tiles = new Tile[tg.size()]();

	
}

void Level::draw()
{

}