#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <Shader.h>
#include <math.h>
#include <Level.h>
#pragma comment(lib, "irrKlang.lib")
#include <Matrix3x3f.h>
#include <Ray3f.h>
#include <GFXEngine.h>

Level level(3,0);
GFXEngine::GFXEngine()//:_raytracer(0),texBoard(0),texBall1(0),texBall2(0)
{
	
}

GFXEngine gfxEngine;
static Shader shader_per_pixel(
"varying vec3 n;"\
"varying vec4 c;"\
"varying vec3 pos;"\
""\
"void main()"\
"{"\
"	n=gl_NormalMatrix*gl_Normal;"\
"	gl_Position=ftransform();"\
"	pos=gl_Position.xyz;"\
"	c=gl_Color;"\
"}"

,
"varying vec3 n;"\
"varying vec3 pos;"\
"varying vec4 c;"\
""\
"void main()"\
"{"\
"	vec3 no=normalize(n);"\
"	if (n.z<0.0)no=-no;"\
"	vec3 H=normalize(vec3(0.0,0.0,1.0)+pos);"\
"	gl_FragColor=vec4(0.125,0.125,0.125,1.0)+max(dot(no,vec3(0.0,0.0,1.0)),0.0)*vec4(0.8,0.8,0.8,1.0)/*+pow(max(dot(no,H),0.0),64.0)*/;"\
"	gl_FragColor*=c;"\
"}"
);

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
int lod = 2;
//TriangleGraph *tg =new TriangleGraph(lod);

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
	//std::cout<<_Delay<<"\n";
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
  	//Vector3f n;
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
	        //n=triangles[i].an()*(1-alpha-beta)+triangles[i].bn()*alpha+triangles[i].cn()*beta;
		}	
		
	}
	if(flag)cpos=hitpoint;else cpos=Vector3f(100000000.f,10000000000.f,100000000000000.f);
	//bool b=_programWindow->snag()->hit(t,_id,ray, hitpoint,n, true);
	return id;
}
void setTile(int i)
{
	switch(i)
	{//empty=0,grass=1,mountain=2,water=3
		case 0:
		{
			level[counter].type=Tile::empty;
			level[counter].height=1.0f;	
		}break;	
		case 1:
		{
			level[counter].type=Tile::grass;
			level[counter].height=1.0f;		
		}break;	
		case 2:
		{
			level[counter].type=Tile::mountain;
			level[counter].height=1.2f;		
		}break;	
		case 3:
		{
			level[counter].type=Tile::water;
			level[counter].height=0.95f;	
		}break;	
		case 4:
		{
			level[counter].type=Tile::stonewall;
			level[counter].height=1.2f;		
		}break;	
		case 5:
		{
			level[counter].type=Tile::way;
			level[counter].height=1.0f;		
		}break;	
		case 6:
		{
			level[counter].type=Tile::street;
			level[counter].height=1.0f;	
		}break;	
		case 7:
		{
			level[counter].type=Tile::woodbridge;
			level[counter].height=1.0f;		
		}break;	
		case 8:
		{
			level[counter].type=Tile::stonebridge;
			level[counter].height=1.0f;		
		}break;	
		case 9:
		{
				
		}break;			
	}
}

static void draw ()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
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
  	shader_per_pixel.use();
	
	//TriangleGraph &T=(*tg);
	const TriangleGraph &T=level.triangleGraph();
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
	}/**/
	gfxEngine.drawStartTile(level, 0);
	gfxEngine.drawPortalTile(level, 0);

	Shader::unuse();
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
	
	}/**/

	SDL_GL_SwapBuffers();
}


int main (int argc, char *argv[])
{
	std::string error;
    if(!level.load("out.lev",error)) std::cout<<"Fehler :\""<<error<<"\"\n";
    int done=0;
    bool _fullScreenToggled=false;
	std::cout<<"init: "<<init("Test01", 0, 800, 600)<<std::endl;
	
	if(!shader_per_pixel.compileAndLink()  )std::cout<<("White-Shader-Error:"  +shader_per_pixel.getLog()  )<<"\n";
    //Sound
	//irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	//irrklang::ISound* SoundEffect = SoundEngine->play2D("Sound_Water0001.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	//SoundEffect->setIsPaused(true);
	//Sound_Water0001.wav
	time_=SDL_GetTicks();
	bool up_pressed=false;
    bool down_pressed=false;
	bool right_pressed=false;
	bool left_pressed=false;
	bool plus_pressed=false;
	bool minus_pressed=false;
	bool num_pressed[10]={false,false,false,false,false,false,false,false,false,false};
	bool numKP_pressed[10]={false,false,false,false,false,false,false,false,false,false};
	bool mousebuttondown[2]={false,false};
	int mx,my;
	while (!done)
    {
		//SDL_Delay(50);
        
		if (frameLimit)SDL_Delay(getDelay());
		else time_=SDL_GetTicks();
		SDL_Event event;
        /* Check for events */
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
						if(tr>=0)counter=tr;
						mousebuttondown[0]=true;
					}
					break;
					case SDL_BUTTON_MIDDLE:
					{
							
					}
						
					break;
					case SDL_BUTTON_RIGHT:
					{
						mousebuttondown[1]=true;
						
					}	
					break;
				}break;
			case SDL_MOUSEBUTTONUP:switch(event.button.button)
				{
					case SDL_BUTTON_LEFT: 
					{
						mousebuttondown[0]=false;
					}
					break;
					case SDL_BUTTON_MIDDLE:
					{
							
					}
						
					break;
					case SDL_BUTTON_RIGHT:
					{
						mousebuttondown[1]=false;
						
					}	
					break;
				}break;
			case SDL_MOUSEMOTION: 
				if (mousebuttondown[0]){
					int tr=pickTriangle(event.motion.x,event.motion.y);
					if(tr>=0)counter=tr;
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
					case SDLK_PLUS:
					case SDLK_KP_PLUS: plus_pressed=false;  break;
					case SDLK_MINUS:
					case SDLK_KP_MINUS:minus_pressed=false; break;
					//case SDLK_m: SoundEffect->setIsPaused(true);break;	
					case SDLK_ESCAPE: done=1;               break;
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
					case SDLK_0: num_pressed[0]=false; break;
					case SDLK_1: num_pressed[1]=false; break;
					case SDLK_2: num_pressed[2]=false; break;
					case SDLK_3: num_pressed[3]=false; break;
					case SDLK_4: num_pressed[4]=false; break;
					case SDLK_5: num_pressed[5]=false; break;
					case SDLK_6: num_pressed[6]=false; break;
					case SDLK_7: num_pressed[7]=false; break;
					case SDLK_8: num_pressed[8]=false; break;
					case SDLK_9: num_pressed[9]=false; break;
				
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
				//	case SDLK_b:	level[counter].blocking=!level[counter].blocking; break;
					case SDLK_PLUS:
					case SDLK_KP_PLUS: plus_pressed=true;  break;
					case SDLK_MINUS:
					case SDLK_KP_MINUS:minus_pressed=true; break;
					//case SDLK_m: SoundEffect->setIsPaused(false);break;	
					case SDLK_ESCAPE: done=1;              break;
					case SDLK_l: wireframe = ! wireframe; break;
					case SDLK_TAB: counter=(counter+1)%80;std::cout<<"c:"<<counter<<"\n";break;
					//case SDLK_PAGEUP: lod++;delete tg; tg = new TriangleGraph(lod);break;
					//case SDLK_PAGEDOWN: lod--;if(lod<0)lod=0;delete tg; tg = new TriangleGraph(lod);break;
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
					case SDLK_0: num_pressed[0]=true; break;
					case SDLK_1: num_pressed[1]=true; break;
					case SDLK_2: num_pressed[2]=true; break;
					case SDLK_3: num_pressed[3]=true; break;
					case SDLK_4: num_pressed[4]=true; break;
					case SDLK_5: num_pressed[5]=true; break;
					case SDLK_6: num_pressed[6]=true; break;
					case SDLK_7: num_pressed[7]=true; break;
					case SDLK_8: num_pressed[8]=true; break;
					case SDLK_9: num_pressed[9]=true; break;
				};
                break;
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }
        if (up_pressed   )rotx+=1.1;
        if (down_pressed )rotx-=1.1;
        if (right_pressed)roty+=1.1;
        if (left_pressed )roty-=1.1;
        if (plus_pressed )level[counter].height+=0.001f;//{scale*=1.05;if (scale>1.0f)scale=1.0f;}
        if (minus_pressed)level[counter].height-=0.001f;//{scale/=1.05;if (scale<0.01f)scale=0.01f;}
        for (int i=0;i<10;i++)
        {
			if (num_pressed[i] && counter>=0 && level[counter].changeable()) setTile(i); 
			if (numKP_pressed[i] && counter>=0 && level[counter].changeable())level[counter].height=0.85f+((float)i)*0.05;
		}
        draw ();
    }
	//if (SoundEngine) SoundEngine->drop();
	level.save("out.lev");
    return 0;
}
