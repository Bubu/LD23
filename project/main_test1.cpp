#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <Shader.h>
#include <math.h>
#include <TriangleGraph.h>
#include <Ray3f.h>
#include <Matrix3x3f.h>
#include <IrrKlang/irrKlang.h>
#include <level.h>
#pragma comment(lib, "irrKlang.lib")

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
float scale=0.5;
bool frameLimit=true;
bool wireframe=false;
Uint32 time_;
Uint32 fps=60;
Vector3f cpos;
int counter=0;
int lod = 2;
TriangleGraph *tg =new TriangleGraph(lod);

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
  	TriangleGraph &triangles=(*tg);
  	for (int i=0;i<tg->size();i++)
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

static void draw ()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if(!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	const float fovy=60.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 1.0f, +tan(fovy*0.017453292519943295769236907684886f)/scale);
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
  	/*glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f,  0.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f,  0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f,  1.0f,  0.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f, -1.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f,  0.0f,  0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f,  0.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();*/
	
	TriangleGraph &T=(*tg);
	glBegin(GL_TRIANGLES);
		for(int i=0;i<T.size();i++)
		{
			if (counter==i)glColor3f(1,1,0); else glColor3f(1,0,0);
			glVertex3f(T[i].a.x, T[i].a.y, T[i].a.z);
			glVertex3f(T[i].b.x, T[i].b.y, T[i].b.z);
			glVertex3f(T[i].c.x, T[i].c.y, T[i].c.z);
		}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0,0,1);

	for(int i=0;i<tg->size();i++)
		{
			
			glVertex3f(T[i].centerPoint().x, T[i].centerPoint().y, T[i].centerPoint().z);
			glVertex3f(T[T[i].n0].centerPoint().x, T[T[i].n0].centerPoint().y, T[T[i].n0].centerPoint().z);
			glVertex3f(T[i].centerPoint().x, T[i].centerPoint().y, T[i].centerPoint().z);
			glVertex3f(T[T[i].n1].centerPoint().x, T[T[i].n1].centerPoint().y, T[T[i].n1].centerPoint().z);
			glVertex3f(T[i].centerPoint().x, T[i].centerPoint().y, T[i].centerPoint().z);
			glVertex3f(T[T[i].n2].centerPoint().x, T[T[i].n2].centerPoint().y, T[T[i].n2].centerPoint().z);
		}
	glEnd();
	Shader::unuse();
	glColor3f(1,1,0);
	glPointSize(11);
	glBegin(GL_POINTS);
		glVertex3f(cpos.x,cpos.y,cpos.z);
	glEnd();
	glPointSize(1);
	SDL_GL_SwapBuffers();
}


int main (int argc, char *argv[])
{
    
    int done=0;
    bool _fullScreenToggled=false;
	std::cout<<"init: "<<init("Test01", 0, 800, 600)<<std::endl;
	
	if(!shader_per_pixel.compileAndLink()  )std::cout<<("White-Shader-Error:"  +shader_per_pixel.getLog()  )<<"\n";
    //Sound
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISound* SoundEffect = SoundEngine->play2D("Sound_Water0001.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	SoundEffect->setIsPaused(true);
	//Sound_Water0001.wav
	time_=SDL_GetTicks();
	bool up_pressed=false;
    bool down_pressed=false;
	bool right_pressed=false;
	bool left_pressed=false;
	bool plus_pressed=false;
	bool minus_pressed=false;
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
				{
					
					int tr=pickTriangle(event.button.x,event.button.y);
					if(tr>=0)counter=tr;
				}
			break;/**/
			/*case SDL_MOUSEMOTION: 
				{
					int tr=pickTriangle(event.motion.x,event.motion.y);
					if(tr>=0)counter=tr;
				}
			break;*/
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
					case SDLK_m: SoundEffect->setIsPaused(true);break;	
					case SDLK_ESCAPE: done=1;               break;
				
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
					case SDLK_PLUS:
					case SDLK_KP_PLUS: plus_pressed=true;  break;
					case SDLK_MINUS:
					case SDLK_KP_MINUS:minus_pressed=true; break;
					case SDLK_m: SoundEffect->setIsPaused(false);break;	
					case SDLK_ESCAPE: done=1;              break;
					case SDLK_l: wireframe = ! wireframe; break;
					case SDLK_TAB: counter=(counter+1)%80;std::cout<<"c:"<<counter<<"\n";break;
					case SDLK_PAGEUP: lod++;delete tg; tg = new TriangleGraph(lod);break;
					case SDLK_PAGEDOWN: lod--;if(lod<0)lod=0;delete tg; tg = new TriangleGraph(lod);break;
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
        if (plus_pressed ){scale*=1.05;if (scale>1.0f)scale=1.0f;}
        if (minus_pressed){scale/=1.05;if (scale<0.01f)scale=0.01f;}
        draw ();
    }
	if (SoundEngine) SoundEngine->drop();
    return 0;
}
