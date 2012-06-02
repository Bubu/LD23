#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <Shader.h>
#include <math.h>
#include <Level.h>
#pragma comment(lib, "irrKlang.lib")
#include <Matrix3x3f.h>
#include <Matrix4x4f.h>
#include <Ray3f.h>
#include <GFXEngine.h>
#include <list>
Level level(3,0);
static const float pi=3.1415926535897932384626433832795f ;

GFXEngine::GFXEngine(){}
GFXEngine gfxEngine;

float rotx=0;
float roty=0;
float rotz=0;
float scale=0.8;
bool frameLimit=true;
bool wireframe=false;
Uint32 time_;
Uint32 fps=60;
Vector3f cpos;
int counter=0;
Matrix3x3f _R;
Vector3f _cameraPos;
float _cameraAngle;
int dist=3;

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
	"	gl_FragColor*=color;//vec4(color.r,color.g,color.b,color.a);\n"\
	"	gl_FragColor.a=color.a;\n"\
	"}"
);

static Uint32 getDelay()
{
	Uint32 _time=SDL_GetTicks();
	Uint32 _optTime=time_+1000/fps;
	int _Delay=_optTime-_time;
	if (_Delay<0)
	{
		_Delay=0;
		_optTime=_time;
	}
	time_=_optTime;
	return _Delay;
}

int pickTriangle(int x, int y)
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
  	const TriangleGraph &triangles=level.triangleGraph();
  	for (int i=0;i<triangles.size();i++)
  	{
		const float h = level[i].height;
		const Vector3f a=triangles[i].a*h-triangles[i].b*h;
		const Vector3f b=triangles[i].a*h-triangles[i].c*h;
		const Vector3f c=ray.d;
		const Vector3f d=triangles[i].a*h-ray.o;
		
		const Vector3f a_x_b=cross(a,b);
		
		const Vector3f c_x_d=cross(c,d);
		const float detM=dot(a_x_b,c);
		if ((detM>0))continue;
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
		}	
		
	}
	if(flag)cpos=hitpoint;else cpos=Vector3f(100000000.f,10000000000.f,100000000000000.f);
	return id;
}

static void moveForward(float f)
{

	Matrix3x3f R=_R*makeRotXMatrix3x3(f*pi*2);
	Vector3f p=Vector3f(R.m01,R.m11,R.m21);
	int t_trinagle =-1;
	const TriangleGraph& triangleGraph = level.triangleGraph();
	for (int i=0;i<triangleGraph.size();i++)
	{
		if (triangleGraph[i].isInside(p))t_trinagle=i;
	}
	
	float diff=level[t_trinagle].height-level[counter].height;
	if (diff <0.0) diff*=-1.0f;
	const bool blocking=diff>0.05||level[t_trinagle].type==Tile::water;
	if (t_trinagle>=0 &&  !blocking)
	{
		counter=t_trinagle;
		_R=R;
	}
}

void setTriangle(int i)
{
	counter=i;
	const TriangleGraph& triangleGraph=level.triangleGraph();
	const Vector3f v=triangleGraph[i].centerPoint().normalize();
	const Vector3f a=triangleGraph[i].a;
	const Vector3f u=cross(v,v-a).normalize();
	const Vector3f w=cross(u,v);
	_R=Matrix3x3f(u.x, v.x, w.x, u.y, v.y, w.y ,u.z, v.z, w.z);
}

void addRoty(float f)
{
	const float ds=f*pi*2;
	_R=_R*makeRotYMatrix3x3(ds);	
}

inline static int minplusone(int a,int b)
{
	return (a<b)?a+1:b+1;	
}

static void draw_camera(float s)
{
	glDisable (GL_CULL_FACE);
	glPushMatrix();
	float m[16]={_R.m00,_R.m10,_R.m20, 0.0f ,_R.m01,_R.m11,_R.m21, 0.0f ,_R.m02,_R.m12,_R.m22, 0.0f ,_R.m01,_R.m11,_R.m21, 1.0f };
    glMultMatrixf(m);
	glBegin(GL_TRIANGLES);
		
		glNormal3f(0,1,0);
		glColor3f(0,0,1);
		glVertex3f(-s/16, 0.01, +s/16);
		glColor3f(1,1,0);
		glVertex3f(0   , 0.01, -s/16);
		glColor3f(0,0,1);
		glVertex3f(+s/16, 0.01, +s/16);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, _cameraPos.z);
		glVertex3f(_cameraPos.x, 0.0f, _cameraPos.z);
		glVertex3f(_cameraPos.x, _cameraPos.y, _cameraPos.z);
	glEnd();
    glTranslatef(_cameraPos.x,_cameraPos.y,_cameraPos.z);
    glRotatef(57.295779513082320876798154814105f*_cameraAngle,1.0f,0.0f,0.0f);
	const float x1=2.0f*0.0075f;
	const float x2=2.0f*0.005f;
	const float z1=2.0f*0.005f;
	const float z2=2.0f*0.01f;
	const float l=sqrt((x2-x1)*(x2-x1)+z1*z1);
	const float nx=(x2-x1)/l;
	const float ny=(z1)/l;
	shader_per_pixel.use();
	
	glBegin(GL_QUADS);
		glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
	  glNormal3f(+0.0f,+0.0f,+1.0f);
	  glVertex3f(-x2,+x2,+z2);glVertex3f(-x2,-x2,+z2);
	  glVertex3f(+x2,-x2,+z2);glVertex3f(+x2,+x2,+z2);
	  
	  glNormal3f(-1.0f,+0.0f,+0.0f);
	  glVertex3f(-x2,-x2,-z2);glVertex3f(-x2,-x2,+z2);
	  glVertex3f(-x2,+x2,+z2);glVertex3f(-x2,+x2,-z2);
	  
	  glNormal3f(+0.0f,+0.0f,-1.0f);
	  glVertex3f(+x1,+x1,-z2-z1);glVertex3f(+x1,-x1,-z2-z1);
	  glVertex3f(-x1,-x1,-z2-z1);glVertex3f(-x1,+x1,-z2-z1);
	  
	  glNormal3f(+1.0f,+0.0f,+0.0f);
	  glVertex3f(+x2,-x2,+z2);glVertex3f(+x2,-x2,-z2);
	  glVertex3f(+x2,+x2,-z2);glVertex3f(+x2,+x2,+z2);
	  
	  glNormal3f(+0.0f,+1.0f,+0.0f);
	  glVertex3f(-x2,+x2,-z2);glVertex3f(-x2,+x2,+z2);
	  glVertex3f(+x2,+x2,+z2);glVertex3f(+x2,+x2,-z2);
	  
	  glNormal3f(+0.0f,-1.0f,+0.0f);
	  glVertex3f(-x2,-x2,+z2);glVertex3f(-x2,-x2,-z2);
	  glVertex3f(+x2,-x2,-z2);glVertex3f(+x2,-x2,+z2);/**/
	  
	  glNormal3f(+0.0f,-nx,+ny);
	  glVertex3f(-x2,-x2,-z2   );glVertex3f(-x1,-x1,-z2-z1);
	  glVertex3f(+x1,-x1,-z2-z1);glVertex3f(+x2,-x2,-z2   );
	  
	  glNormal3f(+0.0f,+nx,-ny);
	  glVertex3f(-x1,+x1,-z2-z1);glVertex3f(-x2,+x2,-z2   );
	  glVertex3f(+x2,+x2,-z2   );glVertex3f(+x1,+x1,-z2-z1);
	  
	  glNormal3f(-nx,+0.0f,+ny);
	  glVertex3f(+x2,-x2,-z2   );glVertex3f(+x1,-x1,-z2-z1);
	  glVertex3f(+x1,+x1,-z2-z1);glVertex3f(+x2,+x2,-z2   );
	  
	  glNormal3f(+nx,+0.0f,+ny);
	  glVertex3f(-x1,-x1,-z2-z1);glVertex3f(-x2,-x2,-z2   );
	  glVertex3f(-x2,+x2,-z2   );glVertex3f(-x1,+x1,-z2-z1);	  
	glEnd();
	glFlush();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		glColor4f(0.25f, 0.25f, 0.25f, 0.2f);
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,+1.0f,0.0f);glVertex3f(-1.0f,-2.0f,0.0f);
		glVertex3f(+1.0f,-1.0f,0.0f);glVertex3f(+1.0f,+2.0f,0.0f);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	Shader::unuse();
	
}

static void draw ()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	if(!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	const float fovy=60.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 0.1f, 10);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	
	glClearColor(0.5, 0.75, 0.5, 1);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  	if(frameLimit)glColor3f(1,0,0);else glColor3f(1,1,1);
  	glTranslated (0, 0, -tan(fovy*0.017453292519943295769236907684886f)/scale);	
  	glRotated (rotx, 1.0f, 0.0f, 0.0f);
  	glRotated (roty, 0.0f, 1.0f, 0.0f);
  	glRotated (rotz, 0.0f, 0.0f, 1.0f);
	const TriangleGraph &T=level.triangleGraph();
	switch(dist)
	{
		case 0:
		{
		
			for (int i=0;i<T.size();i++)
			{
				switch(level[i].type)
				{
					case Tile::empty: 		gfxEngine.drawEmptyTile(level, i, 0);		break; 
					case Tile::grass: 		gfxEngine.drawGrasTile(level, i, 0);		break; 
					case Tile::mountain: 	gfxEngine.drawMountainTile(level, i, 0);	break; 
					case Tile::water: 		gfxEngine.drawWaterTile(level, i, 0);		break; 
					case Tile::stonewall: 	gfxEngine.drawStonewallTile(level, i, 0);	break;  
					case Tile::way: 		gfxEngine.drawWayTile(level, i, 0);			break; 
					case Tile::street: 		gfxEngine.drawStreetTile(level, i, 0);		break;
					case Tile::woodbridge:	gfxEngine.drawWoodbridgeTile(level, i, 0);	break; 
					case Tile::stonebridge:	gfxEngine.drawStonebridgeTile(level, i, 0);	break;	
			
				}
			}
			gfxEngine.drawStartTile(level, 0);
			gfxEngine.drawPortalTile(level, 0);

		
			
		}break;
		case 1:
		{
			
			std::list<int> l;
			l.push_back(counter);
			int di[level.size()];
			for (int i=0;i<T.size();i++)di[i]=-1;
			di[counter]=0;
			while(l.size()>0)
			{
				int tri=l.front();
				if (di[T[tri].n0]<0){l.push_back(T[tri].n0);di[T[tri].n0]=di[tri]+1;}
				if (di[T[tri].n1]<0){l.push_back(T[tri].n1);di[T[tri].n1]=di[tri]+1;}
				if (di[T[tri].n2]<0){l.push_back(T[tri].n2);di[T[tri].n2]=di[tri]+1;}
				l.pop_front();	
			}
			Vector3f color;
			glBegin(GL_TRIANGLES);
			for (int i=0;i<T.size();i++)
			{
				switch(di[i])
				{
					case 0:color=Vector3f(1.0f,0.0f,0.0f);break;
					case 1:color=Vector3f(0.9f,0.0f,0.0f);break;
					case 2:color=Vector3f(0.8f,0.0f,0.0f);break;
					case 3:color=Vector3f(0.7f,0.0f,0.0f);break;
					case 4:color=Vector3f(0.6f,0.0f,0.0f);break;
					case 5:color=Vector3f(0.5f,0.0f,0.0f);break;
					case 6:color=Vector3f(0.4f,0.0f,0.0f);break;
					case 7:color=Vector3f(0.3f,0.0f,0.0f);break;
					case 8:color=Vector3f(0.2f,0.0f,0.0f);break;
					case 9:color=Vector3f(0.1f,0.0f,0.0f);break;
					default: color=Vector3f(0.0f,0.0f,0.0f);
				}
				glColor3f(color.x,color.y,color.z);
				const Vector3f n=cross(T[i].b-T[i].a,T[i].c-T[i].a);
				glNormal3f(n.x,n.y,n.z);
				glVertex3f(T[i].a.x, T[i].a.y, T[i].a.z);
				glVertex3f(T[i].b.x, T[i].b.y, T[i].b.z);
				glVertex3f(T[i].c.x, T[i].c.y, T[i].c.z);
			
			}
			glEnd();
		}break;
		
		case 2:
		{
			int di[level.size()];
			{
				
				const float l=(T[0].a-T[0].b).length();
				const Vector3f m=T[counter].centerPoint();
				const Vector3f v(_R.m01,_R.m11,_R.m21);
				for (int i=0;i<T.size();i++)
				{
					const int a=(int)((v-T[i].a).length()/l);
					const int b=(int)((v-T[i].b).length()/l);
					const int c=(int)((v-T[i].c).length()/l);
					di[i]=a;if (b<di[i])di[i]=b;if (c<di[i])di[i]=c;
					if(di[i]>3)di[i]=3;
				}
			}
			glBegin(GL_TRIANGLES);
			const float l=(T[0].a-T[0].b).length();
			for (int i=0;i<T.size();i++)
			{
				const int d=di[i];
				
				const Vector3f u(_R.m00,_R.m10,_R.m20);
				const Vector3f v(_R.m01,_R.m11,_R.m21);
				const Vector3f w(_R.m02,_R.m12,_R.m22);
				const Vector3f n=cross(T[i].b-T[i].a,T[i].c-T[i].a);
				Matrix3x3f _RT=_R;
				_RT.transpose();
				Matrix3x3f _RCT=makeRotXMatrix3x3(-_cameraAngle);
				const float r=0.2*(float)((6*6+5-d)%6);
				const float g=(dot(v,n)<0)?1:0;
				
				const float b=((dot(_RCT*Vector3f(0,0,1),_RT*T[i].a-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].b-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].c-Vector3f(0,1,0)-_cameraPos)>0))?1:0;
				glColor3f(r,g,b);
				
				glNormal3f(n.x,n.y,n.z);
				glVertex3f(T[i].a.x, T[i].a.y, T[i].a.z);
				glVertex3f(T[i].b.x, T[i].b.y, T[i].b.z);
				glVertex3f(T[i].c.x, T[i].c.y, T[i].c.z);
			
			}
			glEnd();
		}break;
		case 3:
		{
			int di[level.size()];
			{
				
				const float l=(T[0].a-T[0].b).length();
				const Vector3f m=T[counter].centerPoint();
				const Vector3f v(_R.m01,_R.m11,_R.m21);
				for (int i=0;i<T.size();i++)
				{
					const int a=(int)((v-T[i].a).length()/l);
					const int b=(int)((v-T[i].b).length()/l);
					const int c=(int)((v-T[i].c).length()/l);
					di[i]=a;if (b<di[i])di[i]=b;if (c<di[i])di[i]=c;
					if(di[i]>3)di[i]=3;
				}
			}
			
			for (int i=0;i<T.size();i++)
			{
				const int d=di[i];
				
				const Vector3f u(_R.m00,_R.m10,_R.m20);
				const Vector3f v(_R.m01,_R.m11,_R.m21);
				const Vector3f w(_R.m02,_R.m12,_R.m22);
				const Vector3f n=cross(T[i].b-T[i].a,T[i].c-T[i].a);
				const float r=0.2*(float)((6*6+5-d)%6);
				const float g=((dot(v,n)<0))?1:0;
				const float b=((dot(w,T[i].a-v)>0)&&(dot(w,T[i].b-v)>0)&&(dot(w,T[i].c-v)>0))?1:0;
				const float l=(T[0].a-T[0].b).length();
				Matrix3x3f _RT=_R;
				_RT.transpose();
				Matrix3x3f _RCT=makeRotXMatrix3x3(-_cameraAngle);
				bool forward=!(((dot(_RCT*Vector3f(0,0,1),_RT*T[i].a-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].b-Vector3f(0,1,0)-_cameraPos)>0)&&(dot(_RCT*Vector3f(0,0,1),_RT*T[i].c-Vector3f(0,1,0)-_cameraPos)>0)));
				
				bool up=!(dot(v,n)<0);
				bool visible=up&&forward;
				if (visible)switch(level[i].type)
				{
					case Tile::empty: 		gfxEngine.drawEmptyTile(level, i, d);		break; 
					case Tile::grass: 		gfxEngine.drawGrasTile(level, i, d);		break; 
					case Tile::mountain: 	gfxEngine.drawMountainTile(level, i, d);	break; 
					case Tile::water: 		gfxEngine.drawWaterTile(level, i, d);		break; 
					case Tile::stonewall: 	gfxEngine.drawStonewallTile(level, i, d);	break;  
					case Tile::way: 		gfxEngine.drawWayTile(level, i, d);			break; 
					case Tile::street: 		gfxEngine.drawStreetTile(level, i, d);		break;
					case Tile::woodbridge:	gfxEngine.drawWoodbridgeTile(level, i, d);	break; 
					case Tile::stonebridge:	gfxEngine.drawStonebridgeTile(level, i, d);	break;	
			
				}
				else
				{
					glBegin(GL_TRIANGLES);
					glNormal3f(n.x,n.y,n.z);
					glColor3f(1,1,1);
					glVertex3f(T[i].a.x, T[i].a.y, T[i].a.z);
					glVertex3f(T[i].b.x, T[i].b.y, T[i].b.z);
					glVertex3f(T[i].c.x, T[i].c.y, T[i].c.z);
					glEnd();	
				}
			}
			gfxEngine.drawStartTile(level, 0);
			gfxEngine.drawPortalTile(level, 0);
			
		}break;
	}
	
	for(int i=0;i<T.size();i++)
	{
		glBegin(GL_LINE_LOOP);
			const Vector3f m=T[i].centerPoint();
			const Vector3f a=(counter==i)?T[i].a*0.95f+m*0.05f:T[i].a;
			const Vector3f b=(counter==i)?T[i].b*0.95f+m*0.05f:T[i].b;
			const Vector3f c=(counter==i)?T[i].c*0.95f+m*0.05f:T[i].c;
			if (counter==i)glColor3f(1,1,0);else glColor3f(0,0,1);
			const float height_factor = level[i].height;
			glVertex3f(a.x*(0.001+height_factor), a.y*(0.001+height_factor), a.z*(0.001+height_factor));
			glVertex3f(b.x*(0.001+height_factor), b.y*(0.001+height_factor), b.z*(0.001+height_factor));
			glVertex3f(c.x*(0.001+height_factor), c.y*(0.001+height_factor), c.z*(0.001+height_factor));
		glEnd();			
	}
	draw_camera((T[0].a-T[0].b).length());
	
	SDL_GL_SwapBuffers();
}


int main (int argc, char *argv[])
{
	std::string error;
    if(!level.load("out.lev",error)) std::cout<<"Fehler :\""<<error<<"\"\n";
    int done=0;
    bool _fullScreenToggled=false;
	std::cout<<"init: "<<init("Test05", 0, 800, 600)<<std::endl;
	setTriangle(counter);
	time_=SDL_GetTicks();
	bool up_pressed=false;
    bool down_pressed=false;
	bool right_pressed=false;
	bool left_pressed=false;
	bool numKP_pressed[10]={false,false,false,false,false,false,false,false,false,false};
	bool mousebuttondown[2]={false,false};
	int mx,my;
	while (!done)
    {
		if (frameLimit)SDL_Delay(getDelay());
		else time_=SDL_GetTicks();
		SDL_Event event;
        while (SDL_PollEvent (&event))
        {
			
			
            switch (event.type)
            {
			case SDL_MOUSEBUTTONDOWN:
				mx=event.button.x;
				my=event.button.y;
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT: 
					{
						int tr=pickTriangle(event.button.x,event.button.y);
						if(tr>=0){counter=tr;setTriangle(counter);}
						mousebuttondown[0]=true;
					}
					break;
					case SDL_BUTTON_RIGHT:mousebuttondown[1]=true;break;
				}break;
			case SDL_MOUSEBUTTONUP:switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:mousebuttondown[0]=false;break;
					case SDL_BUTTON_RIGHT:mousebuttondown[1]=false;break;
				}break;
			case SDL_MOUSEMOTION: 
				if (mousebuttondown[0]){
					int tr=pickTriangle(event.motion.x,event.motion.y);
					if(tr>=0){counter=tr;setTriangle(counter);}
				}
				if (mousebuttondown[1])
				{
					roty+=0.1*(float)(event.button.x-mx);
					rotx+=0.1*(float)(event.button.y-my);	
				}
				mx=event.button.x;
				my=event.button.y;
			break;
            case SDL_KEYUP:
				switch (event.key.keysym.sym) 
				{
					case SDLK_UP:      up_pressed=false;    break;
					case SDLK_DOWN:    down_pressed=false;  break;
					case SDLK_RIGHT:   right_pressed=false; break;
					case SDLK_LEFT:    left_pressed=false;  break;
					case SDLK_w:       up_pressed=false;    break;
					case SDLK_s:       down_pressed=false;  break;
					case SDLK_d:       right_pressed=false; break;
					case SDLK_a:       left_pressed=false;  break;
					case SDLK_q:       dist=(dist+1)%4;		break;
					case SDLK_KP0: numKP_pressed[0]=false; break; 
					case SDLK_KP1: numKP_pressed[1]=false; break;
					case SDLK_KP2: numKP_pressed[2]=false; break;
					case SDLK_KP3: numKP_pressed[3]=false; break;
					case SDLK_KP4: numKP_pressed[4]=false; break;
					case SDLK_KP5: numKP_pressed[5]=false; break;
					case SDLK_KP6: numKP_pressed[6]=false; break;
					case SDLK_KP7: numKP_pressed[7]=false; break;
					case SDLK_KP8: numKP_pressed[8]=false; break;
					case SDLK_KP9: numKP_pressed[9]=false; break;
				};
                break;
            case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_UP:      up_pressed=true;    break;
					case SDLK_DOWN:    down_pressed=true;  break;
					case SDLK_RIGHT:   right_pressed=true; break;
					case SDLK_LEFT:    left_pressed=true;  break;
					case SDLK_w:       up_pressed=true;    break;
					case SDLK_s:       down_pressed=true;  break;
					case SDLK_d:       right_pressed=true; break;
					case SDLK_a:       left_pressed=true;  break;
					case SDLK_ESCAPE: done=1;              break;
					case SDLK_l: wireframe = ! wireframe; break;
					case SDLK_KP0: numKP_pressed[0]=true; break; 
					case SDLK_KP1: numKP_pressed[1]=true; break;
					case SDLK_KP2: numKP_pressed[2]=true; break;
					case SDLK_KP3: numKP_pressed[3]=true; break;
					case SDLK_KP4: numKP_pressed[4]=true; break;
					case SDLK_KP5: numKP_pressed[5]=true; break;
					case SDLK_KP6: numKP_pressed[6]=true; break;
					case SDLK_KP7: numKP_pressed[7]=true; break;
					case SDLK_KP8: numKP_pressed[8]=true; break;
					case SDLK_KP9: numKP_pressed[9]=true; break;
				};
                break;
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }
        if (up_pressed   )moveForward(+0.04f/fps);
        if (down_pressed )moveForward(-0.04f/fps);
        if (right_pressed)addRoty(+0.3333f/fps);
        if (left_pressed )addRoty(-0.3333f/fps);
        if (numKP_pressed[4])_cameraPos.x+=0.01f;
        if (numKP_pressed[6])_cameraPos.x-=0.01f;
        if (numKP_pressed[8])_cameraPos.y+=0.01f;
        if (numKP_pressed[2])_cameraPos.y-=0.01f;
        if (numKP_pressed[7])_cameraPos.z+=0.01f;
        if (numKP_pressed[9])_cameraPos.z-=0.01f;
        if (numKP_pressed[1])_cameraAngle+=0.01f;
        if (numKP_pressed[3])_cameraAngle-=0.01f;
        if (numKP_pressed[5]){_cameraAngle=0.00f;_cameraPos=Vector3f();}
        
        
        draw ();
    }

    return 0;
}
