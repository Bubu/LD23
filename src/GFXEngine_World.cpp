#include <World.h>
#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <Player.h>
#include <iostream>

void GFXEngine::drawIngame(const World& world, const Player& player)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	const float fovy=90.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 0.01f, 4.000);
  	//gluPerspective(fovy,((float)width)/((float)height), 1.0f, 1000.0f);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	//glTranslated (0, 0, -tan(fovy*0.017453292519943295769236907684886f)/0.5);	
  	float m[16];
  	player.getInverseTransformation(m);
  	const float f=57.295779513082320876798154814105f;
  	const Vector3f campos=player.cameraPos();
  	const float camangle=f*player.cameraAngle();
  	const float roty=f*player.roty();
  	
	  
	  
	  //glTranslatef(0,sin(player.roty()),0);
	  
	  glRotatef(-camangle,1.0f,0.0f,0.0f);
	  glTranslatef(-campos.x,-campos.y,-campos.z);
	  glRotatef(roty,0.0f,1.0f,0.0f);
	  
	  
	  
	  glMultMatrixf(m);
	  
	  ///
	    //
	  //glTranslatef(-campos.x,-campos.y,-campos.z);
  	
  	
  	
  	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	{
		glColor3f(1,1,0);
		glPointSize(11);
		Matrix3x3f m;
		Vector3f p;
		player.getTransformation(p,m);
		p*=1.001f;
		glBegin(GL_POINTS);
			glVertex3f(p.x,p.y,p.z);
		glEnd();
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
			if (player.trinagle()==i)glColor3f(1,1,1);
			glVertex3f(a.x,a.y,a.z);	
			glVertex3f(b.x,b.y,b.z);
			glVertex3f(c.x,c.y,c.z);	
		}
		glEnd();
		drawGenie(world.genie(), player);
		const Projectile& attack=world.attack();
		if (attack.isAlive())
		{
			glColor3f(1,1,1);
			Vector3f p=attack.pos()*1.1;
			glPointSize(11);
			glBegin(GL_POINTS);
				glVertex3f(p.x,p.y,p.z);
			glEnd();
			glPointSize(1);
			std::cout<<"Projectile:("<<p.x<<","<<p.y<<","<<p.z<<"\n";
			
		}
	}
		//world.level(world.current()).draw();
}

void GFXEngine::drawIngamePaused(const World& world,const Player& player, const Menu& menu)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int _width=screen->w;
	const int _height=screen->h;
	
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawIngame(world,player);
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

void GFXEngine::drawGenie(const Genie& genie, const Player& player)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		//glTranslatef(_translate.x,_translate.y,_translate.z);
		const float f=57.295779513082320876798154814105f;
		const float roty=f*player.roty();
		glColor3f(0,0,1);
		const float s=0.05;
		float m[16];
		player.getTransformation(m);
		float h=player.h();
		m[0]*=s;m[1]*=s;m[2]*=s; 
		m[4]*=s;m[5]*=s;m[6]*=s;
		m[8]*=s;m[9]*=s;m[10]*=s;
		glMultMatrixf(m);
		glRotatef(roty,0.0f,1.0f,0.0f);
		glTranslatef(0.0f,h,0.0f);
		Vector3f points[10]; genie.getPositions(points);
		
		glPointSize(11.0f);
		glBegin(GL_POINTS);
			for (int i=0;i<10;i++)glVertex3f(points[i].x,points[i].y,points[i].z);
		glEnd();
		glPointSize(1.0f);
		glBegin(GL_LINES);
			glVertex3f(points[0].x,points[0].y,points[0].z); glVertex3f(points[1].x,points[1].y,points[1].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[1].x,points[1].y,points[1].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[3].x,points[3].y,points[3].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[4].x,points[4].y,points[4].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[7].x,points[7].y,points[7].z);
			glVertex3f(points[4].x,points[4].y,points[4].z); glVertex3f(points[5].x,points[5].y,points[5].z);
			glVertex3f(points[6].x,points[6].y,points[6].z); glVertex3f(points[5].x,points[5].y,points[5].z);
			glVertex3f(points[7].x,points[7].y,points[7].z); glVertex3f(points[8].x,points[8].y,points[8].z);
			glVertex3f(points[9].x,points[9].y,points[9].z); glVertex3f(points[8].x,points[8].y,points[8].z);		
		glEnd();
	glPopMatrix();
}
