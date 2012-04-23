#include <World.h>
#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <GL/glu.h>
#include <Ray3f.h>
#include <SDL/SDL.h>
#include <iostream>



void GFXEngine::drawIngame(const World& world, EventManager evt)
{
	
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	const float fovy=60.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 1.0f, +10*tan(fovy*0.017453292519943295769236907684886f)/0.5);
  	//gluPerspective(fovy,((float)width)/((float)height), 1.0f, 1000.0f);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	glTranslated (0, 0, -tan(fovy*0.017453292519943295769236907684886f)/scale);	
	glRotated (rotx, 1.0f, 0.0f, 0.0f);
  	glRotated (roty, 0.0f, 1.0f, 0.0f);
  	glRotated (rotz, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	{
		glColor3f(0,1,0);
		glPointSize(11);
		glBegin(GL_POINTS);
		glVertex3f(0,-1.01,0);
		glColor3f(1,0,1);
		glVertex3f(0,1.01,0);
		glEnd();
		glPointSize(1);
		const Level& level=world.level(world.current());
		const int n=level.size();

		const TriangleGraph& triangleGraph=level.triangleGraph();
		if(evt.change)
		{
			int tr=pickTriangle(evt.button_x, evt.button_y, triangleGraph);
			if(tr!=counter)
			{
				file<<tr<<"\n";
				level[tr].color = Vector3f(0.7,0.7,0.7);
				level[tr].type = 1;
				level[tr].height = 1.1;

			}
			if(tr>=0)counter=tr;
		}
		if (evt.upPressed())rotx+=1.1;
        if (evt.downPressed())rotx-=1.1;
        if (evt.leftPressed())roty+=1.1;
        if (evt.rightPressed())roty-=1.1;

		glBegin(GL_TRIANGLES);
		for (int i=0;i<n;i++)
		{
			bool active=false;
			if (counter==i) active=true; else active=false;
			switch(level[i].type)
			{
			case 0: { // base tile
					const Vector3f& color=level[i].color;
					const Vector3f& a=triangleGraph[i].a;
					const Vector3f& b=triangleGraph[i].b;
					const Vector3f& c=triangleGraph[i].c;
					glColor3f(color.x,color.y,color.z);
					if(active)glColor3f(1,1,0);
					glVertex3f(a.x,a.y,a.z);	
					glVertex3f(b.x,b.y,b.z);
					glVertex3f(c.x,c.y,c.z); break;}
			case 1: { // mountain tile
					const Vector3f& color=level[i].color;
					const float height_factor = level[i].height;
					const Vector3f& a=triangleGraph[i].a;
					const Vector3f& b=triangleGraph[i].b;
					const Vector3f& c=triangleGraph[i].c;
					glColor3f(color.x,color.y,color.z);
					if(active)glColor3f(1,1,0);
					Vector3f a_new = a * height_factor;
					Vector3f b_new = b * height_factor;
					Vector3f c_new = c * height_factor;
					glVertex3f(a_new.x,a_new.y,a_new.z);	
					glVertex3f(b_new.x,b_new.y,b_new.z);
					glVertex3f(c_new.x,c_new.y,c_new.z);

					glColor3f(0.5,0.5,0.5);
					glVertex3f(a_new.x,a_new.y,a_new.z);
					glVertex3f(b_new.x,b_new.y,b_new.z);
					glVertex3f(0,0,0);
					glVertex3f(a_new.x,a_new.y,a_new.z);
					glVertex3f(c_new.x,c_new.y,c_new.z);
					glVertex3f(0,0,0);
					glVertex3f(b_new.x,b_new.y,b_new.z);
					glVertex3f(c_new.x,c_new.y,c_new.z);
					glVertex3f(0,0,0);break;}
			}
				
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
	//drawIngame(world);
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

int GFXEngine::pickTriangle(int x, int y, const TriangleGraph& tg)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int _width=screen->w;
	const int _height=screen->h;
	const float factor=0.017453292519943295769236907684886f;
	Matrix3x3f r1=makeRotMatrix3x3(rotx*factor, roty*factor, rotz*factor);
	const float fovy=60.0f;
	Vector3f e=(r1*Vector3f(0, 0, +tan(fovy*0.017453292519943295769236907684886f)/scale));
  	Ray3f ray=createRayPerspective(Vector3f(r1.m00,r1.m10,r1.m20),Vector3f(r1.m01,r1.m11,r1.m21),Vector3f(r1.m02,r1.m12,r1.m22), e, _width, _height, (float)x, (float)y, fovy);
	
  	float t=100000.0f;
	bool flag=false;
	int id=-1;
  	Vector3f hitpoint;
  	//Vector3f n;
	const TriangleGraph &triangles=tg;
  	for (int i=0;i<tg.size();i++)
  	{
		const Vector3f a=triangles[i].a-triangles[i].b;
		const Vector3f b=triangles[i].a-triangles[i].c;
		const Vector3f c=ray.d;
		const Vector3f d=triangles[i].a-ray.o;
		
		const Vector3f a_x_b=cross(a,b);
		
		const Vector3f c_x_d=cross(c,d);
		const float detM=dot(a_x_b,c);
		//if ((detM<0)==front)continue;
		const float alpha=dot(c_x_d,b)/detM;
		const float beta=-dot(c_x_d,a)/detM;
		const float _t=dot(a_x_b,d)/detM;
		if (0.0f <= alpha && alpha <=1.0f && 0.0f <= beta && beta <=1.0f &&
		    0.0f <= alpha+beta && alpha+beta <=1.0f && _t<t && _t>0.0)
		{
		 	t=_t;
	        flag=true;
	        id=i;
	        hitpoint=ray.o+ray.d*t;
	        //n=triangles[i].an()*(1-alpha-beta)+triangles[i].bn()*alpha+triangles[i].cn()*beta;
		}	
		
	}
	if(flag)cpos=hitpoint;else cpos=Vector3f(100000000.f,10000000000.f,100000000000000.f);
	//bool b=_programWindow->snag()->hit(t,_id,ray, hitpoint,n, true);
	return id;
}