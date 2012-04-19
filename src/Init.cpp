#include<Init.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>


#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
# pragma comment (lib, "glew32.lib")
# pragma comment (lib, "opengl32.lib") /* link with Microsoft OpenGL lib */
# pragma comment (lib, "glu32.lib") /* link with Microsoft OpenGL Utility lib */


bool init(char *title, bool fullScreen, int width, int height)
{	
	SDL_Surface *screen;
	//std::cout<<"INit: "<<SDL_Init (SDL_INIT_VIDEO| SDL_INIT_JOYSTICK)<<"\n--\n"<<SDL_GetError()<<"++\n";
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK);
	//if (SDL_InitSubSystem (SDL_INIT_VIDEO) < 0) return false;
	atexit (SDL_Quit);
	
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int bpp = info->vfmt->BitsPerPixel;
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	int flags = SDL_OPENGL;
	if (fullScreen)flags = flags ^ SDL_FULLSCREEN;
	
	if ((screen = SDL_SetVideoMode(width, height, bpp, flags)) == 0)return false;
	SDL_WM_SetCaption (title, NULL);
	SDL_Joystick *joystick;
	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);
	
	GLenum err = glewInit();
	//std::cout<<"glewInit()"<<err<<std::endl;
	if (GLEW_OK != err) {
		std::cout << "GLEW error: " << glewGetErrorString(err);
		return false;
	}

	if (!GLEW_VERSION_2_0) {
		std::cout << "OpenGL2 is not supported";
		return false;
	}
	return true;
	
}
