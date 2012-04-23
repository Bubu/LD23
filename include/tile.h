#ifndef __tile_h_included__
#define __tile_h_included__
#include <string>
#include <Vector3f.h>


class Tile
{
	public:
		int type;
		float height;
		Vector3f color;
		inline Tile(std::string Type, Vector3f color):type(type),color(color){}
		inline Tile():type(0),color(Vector3f(0.5,0.5,0.5)),height(1.0),blocking(false),item(-1),doorid(-1){}
		bool blocking;
		int item;
		int doorid;
	private:

};

#endif
