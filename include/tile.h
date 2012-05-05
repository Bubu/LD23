#ifndef __tile_h_included__
#define __tile_h_included__
#include <string>
#include <Vector3f.h>


class Tile
{
	public:
		enum _Type {empty=0,grass=1,mountain=2,water=3,stonewall=4,way=5,street=6,woodbridge=7,stonebridge=8,start=10,portal=11} type;
		float height;
		//Vector3f color;
		//inline Tile(std::string Type, Vector3f color):type(type),color(color){}
		inline Tile():type(empty),height(1.0){}
		inline bool changeable()const{return !(type==start || type==portal);}
		//bool blocking;
		//int item;
		//int doorid;
	private:

};

#endif
