#include <World.h>
#include <GFXEngine.h>
#include <Menu.h>
#include <Shader.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <Player.h>
#include <iostream>
#include <Efreet.h>
//#include <Matrix4x4f.h>
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
static Efreet sefreet;
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
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	float m[16];
  	player.getInverseTransformation(m);
  	const float f=57.295779513082320876798154814105f;
  	const Vector3f campos=player.cameraPos();
  	
  	const float camangle=f*player.cameraAngle();
	  
	glRotatef(-camangle,1.0f,0.0f,0.0f);
	glTranslatef(-campos.x,-campos.y,-campos.z);
	glMultMatrixf(m);
	  
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	{
		glColor3f(1,1,0);
		glPointSize(11);
		Matrix3x3f m;
		Vector3f p;
		player.getTransformation(p,m);
		p*=1.001f;
		glBegin(GL_POINTS);
			for (int i=0;i<1000;i++)glVertex3f(player.DEB_trace[i].x,player.DEB_trace[i].y,player.DEB_trace[i].z);
		glEnd();
		
		const Level& level=world.level(world.current());
		const int n=level.size();
		const TriangleGraph& triangleGraph=level.triangleGraph();
		

		glDisable (GL_CULL_FACE);
		glCullFace(GL_BACK);
		const TriangleGraph &T=level.triangleGraph();
		shader_per_pixel.use();


	const int counter=player.trinagle();
	Matrix3x3f R;
	Vector3f v;
	player.getTransformation(v,R);
	int di[world.currentLevel().size()];
	{
		const float l=(T[0].a-T[0].b).length();
		const Vector3f m=T[counter].centerPoint();
		//const Vector3f v(R.m01,R.m11,R.m21);
		for (int i=0;i<T.size();i++)
		{
			const int a=(int)((v-T[i].a).length()/l);
			const int b=(int)((v-T[i].b).length()/l);
			const int c=(int)((v-T[i].c).length()/l);
			di[i]=a;if (b<di[i])di[i]=b;if (c<di[i])di[i]=c;
			if(di[i]>3)di[i]=3;
		}
	}
		
		
	Shader::unuse();
	const float l=(T[0].a-T[0].b).length();
	for (int i=0;i<T.size();i++)
	{
		const int d=di[i];
		
		const Vector3f u(R.m00,R.m10,R.m20);
		const Vector3f v(R.m01,R.m11,R.m21);
		const Vector3f w(R.m02,R.m12,R.m22);
		const Vector3f n=cross(T[i].b-T[i].a,T[i].c-T[i].a);
		
		Matrix3x3f _RT=R;
		_RT.transpose();
		Matrix3x3f _RCT=makeRotXMatrix3x3(-player.cameraAngle());
		const Vector3f _cameraPos=player.cameraPos();
		bool forward=!(((dot(_RCT*Vector3f(0,0,1),_RT*T[i].a-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].b-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].c-Vector3f(0,1,0)-_cameraPos)>0)));
				
		bool up=!(dot(v,n)<0);
		bool visible=up&&forward;
		
		if (visible)switch(level[i].type)
		{
			case Tile::empty: 		drawEmptyTile(level, i, d);		break; 
			case Tile::grass: 		drawGrasTile(level, i, d);		break; 
			case Tile::mountain: 	drawMountainTile(level, i, d);	break; 
			case Tile::water: 		drawWaterTile(level, i, d);		break; 
			case Tile::stonewall: 	drawStonewallTile(level, i, d);	break;  
			case Tile::way: 		drawWayTile(level, i, d);			break; 
			case Tile::street: 		drawStreetTile(level, i, d);		break;
			case Tile::woodbridge:	drawWoodbridgeTile(level, i, d);	break; 
			case Tile::stonebridge:	drawStonebridgeTile(level, i, d);	break;	
			
		}

	}
	drawStartTile(level, 0);
	drawPortalTile(level, 0);
	
	glEnd();
	
		glDisable (GL_CULL_FACE);
		drawGenie(world.genie(), player);
		const Projectile& attack=world.attack();
		if (attack.isAlive())
		{
			glColor3f(1,1,1);
			Vector3f p=attack.pos()*1.05;
			glPointSize(11);
			glBegin(GL_POINTS);
				glVertex3f(p.x,p.y,p.z);
			glEnd();
			glPointSize(1);

		}
	

	}	
}

void GFXEngine::drawIngamePaused(const World& world,const Player& player, const Menu& menu)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int _width=screen->w;
	const int _height=screen->h;
	
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawIngame(world,player);
	glDisable(GL_CULL_FACE);
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
	glFlush();
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f,0.5f,0.0f);
		glVertex2f(0.4f,0.4+0.22f    );
		glVertex2f(0.4f,0.4+0.38f    );
		glVertex2f(0.6f,0.4+0.3f    );
	glEnd();
	
	glBegin(GL_QUADS);
		
		glColor3f(1.0f,0.0f,0.0f);
		glVertex2f(0.35f,0.25f    );
		glVertex2f(0.65f,0.25f    );
		glVertex2f(0.65f,0.35f    );
		glVertex2f(0.35f,0.35f    );
	glEnd();
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

void GFXEngine::drawEfreet(const Efreet& efreet, const Player& player, const Vector3f& p)
{
	glFlush();
	
	float _phi=atan2(p.y,p.x);
	float _teta=acos(p.z/p.length());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		//glTranslatef(_translate.x,_translate.y,_translate.z);
		const float f=57.295779513082320876798154814105f;
		//const float roty=f*player.roty();
		glColor3f(1,0,0);
		const float s=0.05;
		float m[16];
		//player.getTransformation(m);
		{
			const float su=sin(_teta); const float cu=cos(_teta);
  			const float sv=sin(_phi); const float cv=cos(_phi);
  			m[ 0]=cu*cv; m[ 4]=su*cv; m[ 8]=-sv ; m[12]=su*cv;
  			m[ 1]=cu*sv; m[ 5]=su*sv; m[ 9]= cv ; m[13]=su*sv;
  			m[ 2]=-su  ; m[ 6]= cu  ; m[10]=0.0f; m[14]= cu  ;
  			m[ 3]= 0.0f; m[ 7]= 0.0f; m[11]=0.0f; m[15]= 1.0f;	
		}
		
		float h=player.h();
		m[0]*=s;m[1]*=s;m[2]*=s; 
		m[4]*=s;m[5]*=s;m[6]*=s;
		m[8]*=s;m[9]*=s;m[10]*=s;
		glMultMatrixf(m);
		//glRotatef(roty,0.0f,1.0f,0.0f);
		glTranslatef(0.0f,h,0.0f);
		Vector3f points[10]; efreet.getPositions(points);
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
		glColor4f(1,0,0,0.1);
		glEnable(GL_BLEND);
		//glEnable (GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		const int seed=SDL_GetTicks(); 
		srand(seed);
		glBegin(GL_TRIANGLE_STRIP);
		float a=0.0f;
		for(int j=0;j<10;j++)
		{
			a=0.0f;
			glColor4f(1,0.1*(float)j,0,0.2);
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
		/*glBegin(GL_TRIANGLE_STRIP);
		a=0.0f;
		for(int j=0;j<10;j++)
		{
			a=0.0f;
			glColor4f(1,0.1*(float)j,0,0.2);
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
		glEnd();*/
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
		glColor3f(1,0,0);
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

void GFXEngine::drawGenie(const Genie& genie, const Player& player)
{
	glFlush();
	
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		const float f=57.295779513082320876798154814105f;
		glColor3f(0,0,1);
		const float s=0.05;
		float m[16];
		player.getTransformation(m);
		float h=player.h();
		m[0]*=s;m[1]*=s;m[2]*=s; 
		m[4]*=s;m[5]*=s;m[6]*=s;
		m[8]*=s;m[9]*=s;m[10]*=s;
		glMultMatrixf(m);
		glRotatef(180,0.0f,1.0f,0.0f);
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
		//glEnable (GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		const int seed=SDL_GetTicks(); 
		srand(seed);
		glBegin(GL_TRIANGLE_STRIP);
		float a=0.0f;
		for(int j=0;j<5;j++)
		{
			a=0.0f;
			glColor4f(0,0.2*(float)j,1,0.2);
			const float f=0.1+0.09*(float)j;
			for (int i=0;i<500;i++)
			{
				const float h=0.002f*(float)(i);
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
		glEnd();/**/
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

void GFXEngine::drawKey(const Vector3f p)
{
	//p*=1.1;
	Vector3f p2=p*1.02;
	glColor3f(1,1,0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		shader_per_pixel.use();
		glTranslatef(p2.x,p2.y,p2.z);
		const float scale=0.02;
		glScalef(scale,scale,scale);
		glRotatef(20,0,1,0);
		drawSphere(Vector3f(1,+0.1,0),0.2);
		drawSphere(Vector3f(0.7,+0.1,0.05),0.2);
		drawSphere(Vector3f(-1,0,0),0.3);
		//drawSphere(Vector3f(0,0,0));
		glScalef(1,0.2,0.2);
		drawSphere(Vector3f(0,0.5,0),1);
		Shader::unuse();
	glPopMatrix();
	
}
