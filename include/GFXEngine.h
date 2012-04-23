#ifndef __GFXEngine_H_INCLUDED__
#define __GFXEngine_H_INCLUDED__
#include <string>
#include <EventManager.h>
#include <Vector3f.h>
#include <fstream>
//#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>

//class TTF_Font;
//class SDL_Color;
//class SDL_Rect;	

class TriangleGraph;
class Shader;
class Menu;
class World;
//class Texture;
class GFXEngine
{
  public:
	GFXEngine();
	//bool init(Settings * settings);
	void drawMenu(const Menu& menu);
	//void drawHUD(const TacBall& tacBall);
	void drawIngame(const World& world, EventManager evt);
	//void drawIngameFixFunction(const TacBall& tacBall);
	//void drawIngameShader(const TacBall& tacBall);
	
	//void drawBoard(const TacBall& tacBall);//
	void drawIngamePaused(const World& world, const Menu& menu);
	void drawOutro();
	void frameEnd();
	
	void toggleFullscreen();

	int pickTriangle(int x, int y,  const TriangleGraph& tg);
	float rotx;
	float roty;
	float rotz;
	float scale;
	int counter, countercounter;
	Vector3f cpos;
	std::ofstream file;
	void writecountercounter();
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
