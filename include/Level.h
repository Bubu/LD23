#ifndef __Level_h_included__
#define __Level_h_included__
#include <tile.h>

class Level
{
  public:
	  int id;
	  Tile* tiles;
	  int size;
	  Level(int n);
  private:

};

#endif