#ifndef __GFXEngine_H_INCLUDED__
#define __GFXEngine_H_INCLUDED__
#include <string>
#include <Vector3f.h>
//#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>

//class TTF_Font;
//class SDL_Color;
//class SDL_Rect;	

class Efreet;
class Shader;
class Menu;
class World;
class Player;
class Genie;
class Level;
class GFXEngine
{
  public:
	GFXEngine();
	//bool init(Settings * settings);
	void drawMenu(const Menu& menu);
	//void drawHUD(const TacBall& tacBall);
	void drawIngame(const World& world, const Player& player);
	//void drawIngameFixFunction(const TacBall& tacBall);
	//void drawIngameShader(const TacBall& tacBall);
	
	//void drawBoard(const TacBall& tacBall);//
	void drawIngamePaused(const World& world, const Player& player, const Menu& menu);
	void drawOutro();
	void frameEnd();
	
	void toggleFullscreen();
	void drawGenie(const Genie& genie, const Player& player);
	void drawEfreet(const Efreet& efreet, const Player& player, const Vector3f& pos);
	void drawKey(const Vector3f p);
	//lod: 0 near 1 middle 2 far
	void drawEmptyTile(const Level& level, int t, int lod);
	void drawGrasTile(const Level& level, int t, int lod);
	void drawMountainTile(const Level& level, int t, int lod);
	void drawWaterTile(const Level& level, int t, int lod);
	void drawStonewallTile(const Level& level, int t, int lod);
	void drawWayTile(const Level& level, int t, int lod);
	void drawStreetTile(const Level& level, int t, int lod);
	void drawWoodbridgeTile(const Level& level, int t, int lod);
	void drawStonebridgeTile(const Level& level, int t, int lod);
	void drawStartTile(const Level& level, int lod);
	void drawPortalTile(const Level& level, int lod);
  protected:
	
	//int nextpoweroftwo(int x);
	//std::string	convertInt(int number);
	/*void SDL_GL_RenderText(std::string stringtext, 
                      TTF_Font *font,
                      SDL_Color *color,
                      SDL_Rect *location);	*/	
		
	//Settings * _settings;
	int _width;
	int _height;
	//Shader* _raytracer;
	//Texture* texBoard, *texBall1, *texBall2;	
};
#endif
