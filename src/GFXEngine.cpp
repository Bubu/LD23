#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>
#include <GL/glu.h>
#include <iostream>
//#include <GFX/Texture.h>

 # pragma comment (lib, "opengl32.lib") /* link with Microsoft OpenGL lib */
# pragma comment (lib, "glu32.lib") /* link with Microsoft OpenGL Utility lib */
//#include <iostream>

GFXEngine::GFXEngine()//:_raytracer(0),texBoard(0),texBall1(0),texBall2(0)
{
	file.open("tiles.txt");
}


void GFXEngine::drawMenu(const Menu& menu)
{
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int _width=screen->w;
	const int _height=screen->h;

	glViewport(0, 0, _width, _height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	for (int i=0;i<2;i++)
	{
		glColor3f(0.5f,0.0f,0.5f);
		if (menu.getStateMain()==i)	glColor3f(0.0f,1.0f,0.0f);
		float h=((float)(i))*0.4f+0.2f;
		
		glVertex2f(0.2f,h     ); glVertex2f(0.2f,h+0.2f);
		glVertex2f(0.8f,h+0.2f); glVertex2f(0.8f,h     );			
	}
	
	glEnd();
	
	/*glBegin(GL_LINES);
		glVertex2f(1000,1000); glVertex2f(-1000,-1000);
	glEnd();*/
/**/
	
	// Beschriftung
/*	int FontSize = _height/ 40;

	// load font.ttf at size 16 into font
	TTF_Font *font;
	font=TTF_OpenFont("ds9.ttf", FontSize);
	if(!font) {
	printf("TTF_OpenFont: %s\n", TTF_GetError());
	// handle error
	}
	
	SDL_Rect * location = new SDL_Rect();
	SDL_Color * color = new SDL_Color();

  	glViewport(0, 0, _width, _height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _width, 0, _height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	location->w = 8 * FontSize;
	location->h = ((int)(1.6f * FontSize));
	location->x = 10;
	location->y = _height - ((int)(1.5f * location->h));
	color->r = 255;
	color->g = 0;
	color->b = 0;
	
	std::string TextString = "Ehrenvoll antreten!";	
	SDL_GL_RenderText(TextString, font, color, location);
	color->r = 0;
	color->g = 255;
	color->b = 255;
	location->y -= location->h;
	TextString = "Feige fliehen?";
	SDL_GL_RenderText(TextString, font, color, location);
	
	delete location;
	delete color;
	delete font;
*/	
}




void GFXEngine::drawOutro()
{
	//Screen 
	glViewport(0, 0, _width, _height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _width, 0, _height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	
	glColor3f(0.5f,0.5f,0.5f);
	
	glVertex2f(100.0f,100.0f);
	glVertex2f(500.0f,500.0f);
	glVertex2f(500.0f,100.0f);
		
	glVertex2f(100.0f,100.0f);
	glVertex2f(100.0f,500.0f);
	glVertex2f(500.0f,500.0f);

	glEnd();
		
}

void GFXEngine::toggleFullscreen()
{				
	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_SetVideoMode(screen->w, screen->h, screen->format->BitsPerPixel, screen->flags^ SDL_FULLSCREEN);	
}

