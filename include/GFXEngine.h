#ifndef __GFXEngine_H_INCLUDED__
#define __GFXEngine_H_INCLUDED__
#include <string>
//#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>

//class TTF_Font;
//class SDL_Color;
//class SDL_Rect;	


class Shader;
class Menu;
class World;
class Player;
class Genie;
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
  protected:
	void drawGenie(const Genie& genie, const Player& player);
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
