#include <World.h>
#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <Player.h>
#include <iostream>
static Shader shader_per_pixel
(
	"varying vec3 n;\n"\
	"varying vec3 light;"\
	"varying vec4 color;\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"   color=gl_Color;\n"\
	"	n=gl_NormalMatrix*gl_Normal;\n"\
	"	gl_Position=ftransform();\n"\
	"	light=((gl_Vertex+vec4(0.0,0.0,1.0,1.0))*gl_ModelViewProjectionMatrix).xyz;"\
	"}"
	,
	"varying vec3 n;\n"\
	"varying vec3 light;\n"\
	"varying vec4 color;\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"	vec3 no=normalize(n);\n"\
	"//	if (n.z<0.0)no=-no;\n"\
	"	vec3 H=normalize(light);\n"\
	"	gl_FragColor=vec4(0.4,0.4,0.4,1.0)+max(dot(no,vec3(0.0,0.0,1.0)),0.0)*vec4(0.2,0.2,0.2,1.0)+pow(max(dot(no,H),0.0),2.0)*vec4(0.2,0.2,0.2,1.0);\n"\
	"	gl_FragColor.a=1.0f;\n"\
	"	gl_FragColor*=color;//vec4(color.r,color.g,color.b,color.a);\n"\
	"}"
);
static TriangleGraph sphere(0);	
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
			const float height_factor = level[i].height;
			const Vector3f& a=triangleGraph[i].a;
			const Vector3f& b=triangleGraph[i].b;
			const Vector3f& c=triangleGraph[i].c;
			glColor3f(color.x,color.y,color.z);
			if (player.trinagle()==i)glColor3f(1,1,1);
			//if(active)glColor3f(1,1,0);
			Vector3f a_new = a * height_factor;
			Vector3f b_new = b * height_factor;
			Vector3f c_new = c * height_factor;
			glVertex3f(a_new.x,a_new.y,a_new.z);	
			glVertex3f(b_new.x,b_new.y,b_new.z);
			glVertex3f(c_new.x,c_new.y,c_new.z);

			//glColor3f(0.5,0.5,0.5);
			glVertex3f(a_new.x,a_new.y,a_new.z);
			glVertex3f(b_new.x,b_new.y,b_new.z);
			glVertex3f(0,0,0);
			glVertex3f(a_new.x,a_new.y,a_new.z);
			glVertex3f(c_new.x,c_new.y,c_new.z);
			glVertex3f(0,0,0);
			glVertex3f(b_new.x,b_new.y,b_new.z);
			glVertex3f(c_new.x,c_new.y,c_new.z);
			glVertex3f(0,0,0);			
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
void drawSphere(Vector3f m, float s)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m.x,m.y,m.z);
	glScalef(s,s,s);
	glBegin(GL_TRIANGLES);
	for (int i=0;i<sphere.size();i++)
	{
		glNormal3f(sphere[i].a.x,sphere[i].a.y,sphere[i].a.z);
		glVertex3f(sphere[i].a.x,sphere[i].a.y,sphere[i].a.z);
		glNormal3f(sphere[i].b.x,sphere[i].b.y,sphere[i].b.z);
		glVertex3f(sphere[i].b.x,sphere[i].b.y,sphere[i].b.z);
		glNormal3f(sphere[i].c.x,sphere[i].c.y,sphere[i].c.z);
		glVertex3f(sphere[i].c.x,sphere[i].c.y,sphere[i].c.z);
	}
	glEnd();
	glPopMatrix();
	
}
void GFXEngine::drawGenie(const Genie& genie, const Player& player)
{
	glFlush();
	
	
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
		shader_per_pixel.use();
		/*glPointSize(11.0f);
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
		glEnd();*/
		glFlush();
		glColor4f(0,0,1,0.1);
		glEnable(GL_BLEND);
		glEnable (GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		const int seed=SDL_GetTicks(); 
		srand(seed);
		glBegin(GL_TRIANGLE_STRIP);
		float a=0.0f;
		for(int j=0;j<10;j++)
		{
			a=0.0f;
			glColor4f(0,0.1*(float)j,1,0.2);
			const float f=0.1+0.09*(float)j;
			for (int i=0;i<1000;i++)
			{
				const float h=0.001f*(float)(i);
				const float da=0.0001f*(float)(rand()%1000);
				const float r=f*h*(0.3+(0.0002f*(float)(rand()%1000)));
				const float dh=0.2-0.001*0.1*(float)(rand()%1000);
				a+=da;
				glVertex3f(cos(a)*r,(h+dh)*(h+dh),sin(a)*r);		
			}
		}
		glEnd();
		glFlush();	
		glCullFace(GL_FRONT);
		srand(seed);
		glBegin(GL_TRIANGLE_STRIP);
		a=0.0f;
		for(int j=0;j<10;j++)
		{
			a=0.0f;
			glColor4f(0,0.1*(float)j,1,0.2);
			const float f=0.1+0.09*(float)j;
			for (int i=0;i<1000;i++)
			{
				const float h=0.001f*(float)(i);
				const float da=0.0001f*(float)(rand()%1000);
				const float r=f*h*(0.3+(0.0002f*(float)(rand()%1000)));
				const float dh=0.2-0.001*0.1*(float)(rand()%1000);
				a+=da;
				glVertex3f(cos(a)*r,(h+dh)*(h+dh),sin(a)*r);		
			}
		}
		glEnd();
		/**/
		/*for (int i=0;i<10000;i++)
		{
			const float h=0.0001f*(float)(i);
			const float da=0.0001f*(float)(rand()%1000);
			const float r=h*(0.3+(0.0002f*(float)(rand()%1000)));
			const float dh=0.1-0.001*0.05*(float)(rand()%1000);
			a+=da;
			glVertex3f(cos(a)*r,(h+dh)*(h+dh),sin(a)*r);		
		}*/
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		const float pi=3.1415926535897932384626433832795f;
		const float da=(2*pi)/10.0f;
		glBegin(GL_QUADS);
		for (int i=0;i<10;i++)
		{
			const float y1=0.1*(float)(i);
			const float y2=0.1*(float)(i+1);
			const float x1=sqrt(sqrt(1.0f-y1))*0.3f;
			const float x2=sqrt(sqrt(1.0f-y2))*0.3f;
			//std::cout<<"x1:"<<x1<<"\n";
			const float y=points[1].y+0.2f;
			const float f=(points[1].y-points[0].y)*0.8;
			for (float a=0;a<pi*2.0;a+=da)
			{
				const float s1=sin(x1); const float c1=cos(x1); 
				const float s2=sin(x2); const float c2=cos(x2);
				glNormal3f(sin(a   ),0,cos(a   ));
				glVertex3f(x1*sin(a   ),y1*f+y,x1*cos(a   ));
				glVertex3f(x2*sin(a   ),y2*f+y,x2*cos(a   ));
				glNormal3f(sin(a+da),0,cos(a+da));
				glVertex3f(x2*sin(a+da),y2*f+y,x2*cos(a+da));
				glVertex3f(x1*sin(a+da),y1*f+y,x1*cos(a+da));
			}
		}
		glEnd();
		drawSphere(points[3], 0.15);
		drawSphere(points[7], 0.15);
		drawSphere(points[4], 0.15);
		drawSphere(points[9], 0.10);
		drawSphere(points[8], 0.05);
		drawSphere(points[5], 0.05);
		drawSphere(points[6], 0.10);
		Shader::unuse();
		glBegin(GL_LINES);
			glVertex3f(points[7].x,points[7].y,points[7].z); glVertex3f(points[8].x,points[8].y,points[8].z);
			glVertex3f(points[9].x,points[9].y,points[9].z); glVertex3f(points[8].x,points[8].y,points[8].z);
			glVertex3f(points[4].x,points[4].y,points[4].z); glVertex3f(points[5].x,points[5].y,points[5].z);
			glVertex3f(points[6].x,points[6].y,points[6].z); glVertex3f(points[5].x,points[5].y,points[5].z);
		glEnd();
		
		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	glPopMatrix();
}
