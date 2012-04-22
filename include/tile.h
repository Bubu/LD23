#ifndef __tile_h_included__
#define __tile_h_included__
#include <string>
#include <Vector3f.h>


class Tile
{
	public:
		std::string type;
		Vector3f color;
		inline Tile(std::string Type, Vector3f color):type(type),color(color){}
		inline Tile():type("blub"),color(Vector3f(1,0,0)){}
	private:

};

#endif