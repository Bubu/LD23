#ifndef __Level_h_included__
#define __Level_h_included__
#include <tile.h>
#include <TriangleGraph.h>
#include <string>

class Level
{
  public:
	  const int id;
	  
	  //int size;
	  Level(int n, int id);
	  ~Level();
	  //void draw()const;
	  inline Tile& operator[](int i)const{return _tiles[i];}
	  inline int size()const{return _size;}
	  inline const TriangleGraph& triangleGraph()const{return _tg;}
	  void save(std::string file)const;
	  bool load(std::string file, std::string& error);
	  inline int startTile()const{return 997;}
	  inline int portalTile(int i)const
	  {
		switch(i)
		{
			case 0: return 655;
			case 1: return 671;
			case 2: return 693;
			case 3: return 1103;
			case 4: return 1119;
			case 5: return 1141;
		}	
		return -1;	
	  }
  private:
	Tile* _tiles;
	
	TriangleGraph _tg;
	const int _size;
	const int _n;
	
};

#endif
