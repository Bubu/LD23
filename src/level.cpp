#include <level.h>
#include <tile.h>
#include <math.h>
#include <iostream>

Level::Level(int n)
{
	size = n;
	tiles = new Tile[size]();
}