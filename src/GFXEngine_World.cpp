#include <World.h>
#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

void GFXEngine::drawIngame(const World& world)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	const float fovy=60.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 1.0f, +tan(fovy*0.017453292519943295769236907684886f)/0.5);
  	//gluPerspective(fovy,((float)width)/((float)height), 1.0f, 1000.0f);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	glTranslated (0, 0, -tan(fovy*0.017453292519943295769236907684886f)/0.5);	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	{
		
		const Level& level=world.level(world.current());
		const int n=level.size();
		const TriangleGraph& triangleGraph=level.triangleGraph();
		glBegin(GL_TRIANGLES);
		for (int i=0;i<n;i++)
		{
			const Vector3f& color=level[i].color;
			const Vector3f& a=triangleGraph[i].a;
			const Vector3f& b=triangleGraph[i].b;
			const Vector3f& c=triangleGraph[i].c;
			glColor3f(color.x,color.y,color.z);
			glVertex3f(a.x,a.y,a.z);	
			glVertex3f(b.x,b.y,b.z);
			glVertex3f(c.x,c.y,c.z);	
		}
		glEnd();
	}
		//world.level(world.current()).draw();
}

void GFXEngine::drawIngamePaused(const World& world, const Menu& menu)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int _width=screen->w;
	const int _height=screen->h;
	
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawIngame(world);
	glEnable(GL_BLEND);
	glBlendFunc ( GL_SRC_ALPHA    , GL_ONE_MINUS_SRC_ALPHA     );
	//Screen 
	glViewport(0, 0, _width, _height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	glBegin(GL_TRIANGLES);
	
	glColor4f(0.5f,0.0f,0.0f,0.5f);
	
	glVertex2f(0.1f,0.1f);
	glVertex2f(0.9f,0.9f);
	glVertex2f(0.9f,0.1f);
		
	glVertex2f(0.1f,0.1f);
	glVertex2f(0.1f,0.9f);
	glVertex2f(0.9f,0.9f);

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	for (int i=0;i<2;i++)
	{
		glColor3f(0.5f,0.0f,0.5f);
		if (menu.getStatePause()==i)	glColor3f(0.0f,1.0f,0.0f);
		float h=((float)(i))*0.4f+0.2f;
		
		glVertex2f(0.2f,h     ); glVertex2f(0.2f,h+0.2f);
		glVertex2f(0.8f,h+0.2f); glVertex2f(0.8f,h     );			
	}
	glEnable(GL_DEPTH_TEST);
	glEnd();
}
